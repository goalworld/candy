#include "candy/candy_cycle_buffer.h"
#include <string.h>
#include <stdlib.h>
#define DEFALUT_BUF_SZ 1024
static void _grow(struct candy_cycle_buffer* self,int size);
void candy_cycle_buffer_init(struct candy_cycle_buffer* self){
	self->data_ = (unsigned char*)malloc(DEFALUT_BUF_SZ);
	self->start_ = 0;
	self->end_ = 0;
	self->size_ = DEFALUT_BUF_SZ;
	self->has_data_ = 0;
}
void candy_cycle_buffer_destroy(struct candy_cycle_buffer* self){
	free(self->data_);
}
int candy_cycle_buffer_read(struct candy_cycle_buffer* self,void *buf,int sz){
	if(!self->has_data_){
		return 0;
	}
	int nw = candy_cycle_buffer_data_size(self);
	nw = nw < sz ? nw : sz;
	if( (self->start_ >= self->end_) && (self->size_-self->start_ < nw)){
		int first_sz = self->size_-self->start_;
		memcpy(buf,self->data_+self->start_ ,first_sz);
		int left_nw = nw - first_sz;
		buf = (unsigned char*)buf + first_sz;
		memcpy(buf,self->data_ ,left_nw);
		self->start_ = left_nw;

	}else{
		memcpy(buf,self->data_ + self->start_,nw);
		self->start_ += nw;
		if(self->start_ == self->size_){
			self->start_ = 0;
		}
	}
	if(self->end_ == self->start_){
		self->has_data_ = 0;
		self->start_ = self->end_ = 0;
	}
	return nw;
}
void candy_cycle_buffer_write(struct candy_cycle_buffer* self,void *buf,int sz){
	int free_size = candy_cycle_buffer_free_size(self); 
	if(free_size < sz){
		int grow_size = ((sz-free_size)%1024 + 1)*1024;
		_grow(self,grow_size);
	}
	if(self->size_ - self->end_ >= sz){
		memcpy(self->data_+self->end_,buf,sz);
		self->end_ += sz;
		if(self->end_ == self->size_){
			self->end_ = 0;
		}
	}else{
		int left_sz = self->size_ - self->end_;
		memcpy(self->data_+self->end_,buf,left_sz);
		memcpy(self->data_,(unsigned char*)(buf) + left_sz,sz-left_sz);
		self->end_ = sz -left_sz;
	}
	self->has_data_ = 1;
}
int candy_cycle_buffer_free_size(struct candy_cycle_buffer* self){
	if(!self->has_data_){
		return self->size_;
	}
	if(self->start_ > self->end_){
		return (self->start_ - self->end_);
	}else if(self->start_ < self->end_){
		return self->size_ - (self->end_ - self->start_);
	}else{
		return 0;
	}
}
int candy_cycle_buffer_data_size(struct candy_cycle_buffer* self){
	return self->size_ - candy_cycle_buffer_free_size(self);
}


void _grow(struct candy_cycle_buffer* self,int size){
	unsigned char * new_buf = (unsigned char*)malloc(self->size_+size);
	int nw = candy_cycle_buffer_read(self,new_buf,self->size_+size);
	free(self->data_);
	self->data_ = new_buf;
	self->start_ = 0;
	self->end_ = nw;
	self->size_ += size;
	if(nw > 0){
		self->has_data_ = 1;
	}
}


int candy_cycle_buffer_write_from_reader(struct candy_cycle_buffer* self,candy_io_fn reader,void *arg){
	int all_read = 0;
	int nr = 0;
	while(1){
		if(candy_cycle_buffer_free_size(self) < 1024){
			_grow(self,1024);
		}
		if(self->start_ > self->end_){//------end<---->start----
			int buf_sz = self->start_ - self->end_;
			nr = reader(arg,self->data_+self->end_,buf_sz);
			if(nr > 0){
				all_read +=nr;
				self->end_ += nr;
				if(buf_sz == nr){
					continue;
				}
			}
			break;
		}else{//start------end<---------->
			int buf_sz = self->size_ - self->end_;
			nr = reader(arg,self->data_ + self->end_,buf_sz);
			if(!self->has_data_ && self->start_ == self->end_ && nr > 0){
				self->has_data_ = 1;
			}
			if(nr == buf_sz){
				all_read += nr;
				self->end_ = 0;
				continue;
			}else{
				if(nr > 0){
					self->end_ += nr;
					all_read+=nr;
				}
				break;
				//LOG(INFO) << "START:"<<self->start_<<"END:"<<self->end_;
				
			}
		}
	}
	if(all_read == 0){
		return nr;
	}
	return all_read;
}
int candy_cycle_buffer_read_to_writer(struct candy_cycle_buffer* self,candy_io_fn writer,void *arg){
	if(!self->has_data_){
		return 0;
	}
	int nw = 0;
	if(self->start_ >= self->end_){ //== is full 
		int first_sz = self->size_ - self->start_;
		nw = writer(arg,self->data_+self->start_,first_sz);
		if(nw < first_sz){
			return nw;
		}else{
			self->start_ = 0;
			if(self->end_ ==0){
				self->has_data_ = 0;
				return first_sz;
			}
			int second_sz = self->end_;
			nw =  writer(arg,self->data_,second_sz);
			if(nw > 0){
				self->start_ = nw;
				if(self->start_ == self->end_){
					self->start_ = 0;
					self->end_ = 0;
					self->has_data_ = 0;
				}
				return nw+first_sz;
			}else{
				return first_sz;
			}
		}
	}else{
		int sz = self->end_ - self->start_;
		nw = writer(arg,self->data_+self->start_,sz);
		if(nw > 0){
			self->start_ += nw;
			if(self->start_ == self->end_){
				self->start_ = 0;
				self->end_ = 0;
				self->has_data_ = 0;
			}
		}
		return nw;
	}
}