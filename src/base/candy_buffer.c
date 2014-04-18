#include "candy/candy_buffer.h"
#include "candy/candy_log.h"
#include <stdlib.h>
#include <memory.h>
static void _grow(struct candy_buffer* buf,int size);
void candy_buffer_init(struct candy_buffer* buf,int size){
	CANDY_CHECK(buf != NULL);
	CANDY_CHECK(size > 0);
	buf->data = (char*)malloc(size);
	buf->rd = 0;
	buf->wr = 0;
	buf->size = size;
}
void candy_buffer_write(struct candy_buffer* buf,void *data,int size){
	CANDY_CHECK(buf != NULL);
	CANDY_CHECK(data != NULL);
	CANDY_CHECK(size > 0);
	int free_size = candy_buffer_free_size(buf);
	int rd_size = candy_buffer_rd_size(buf);
	int emp_size = free_size+rd_size;
	if(free_size >= size){
		memcpy(buf->data + buf->wr,data,size);
		buf->wr+=size;
	}else{
		if(emp_size < size){
			_grow(buf,size-emp_size);
		}
		memmove(buf->data, buf->data + buf->wr, candy_buffer_data_size(buf));
		buf->wr -= buf->rd;
		buf->rd = 0;
		memcpy(buf->data + buf->wr,data,size);
		buf->wr+=size;
	}
}
void _grow(struct candy_buffer* buf,int size){
	int add_size = 1024;
	while(size > add_size){
		add_size+=1024;
	}
	void* new_buf = realloc(buf->data, buf->size+add_size);
	buf->data = (char*)new_buf;
	buf->size+=add_size;
}
int candy_buffer_read(struct candy_buffer* buf,void *data,int size){
	CANDY_CHECK(buf != NULL);
	CANDY_CHECK(data != NULL);
	CANDY_CHECK(size > 0);
	int data_size = candy_buffer_data_size(buf);
	size = data_size > size ? size : data_size;
	if(size > 0){
		memcpy(data, (void*)(buf->data+buf->rd), size);
	}
	return size;
}
int candy_buffer_read_to_writer(struct candy_buffer* buf,candy_buffer_writer_fn fn,void* arg){
	CANDY_CHECK(buf != NULL);
	CANDY_CHECK(fn != NULL);
	int size = candy_buffer_data_size(buf);
	if(size > 0){
		int ret = fn(arg,(void*)(buf->data+buf->rd),size);
		if(ret > 0){
			buf->rd+=ret;
		}
	}
	return 0;
}

void candy_buffer_destroy(struct candy_buffer* buf){
	CANDY_CHECK(buf != NULL);
	free(buf->data);
}

void candy_buffer_clear(struct candy_buffer* buf){
	buf->rd = 0;
	buf->wr = 0;
}