
#include "candy_poller.h"
#include "candy/candy_log.h"
#include <string.h>

void candy_poller_init(struct candy_poller* self){
	FD_ZERO(&self->rd_set);
	FD_ZERO(&self->wr_set);
	FD_ZERO(&self->err_set);
	candy_array_init(&self->arr,sizeof(void*));
}
void candy_poller_destroy(struct candy_poller* self){
	FD_ZERO(&self->rd_set);
	FD_ZERO(&self->wr_set);
	FD_ZERO(&self->err_set);
	candy_array_destroy(&self->arr);
}
void candy_poller_add(struct candy_poller* self,struct candy_poller_event* ev){
	ev->poller = self;
	candy_array_push(&self->arr,(void*)&ev);
}
void candy_poller_remove(struct candy_poller* self,struct candy_poller_event* ev){
	int size = candy_array_size(&self->arr);
	struct candy_poller_event* cur;
	int i = 0;
	while(i<size){
		candy_array_at(&self->arr,i,&cur);
		if(cur == ev){
			candy_array_earse(&self->arr,i,1,NULL);
			if(i <= self->cur && self->cur != 0){
				self->cur--;
			}
			return;
		}
		i++;
	}
	
}
void candy_poller_set_in(struct candy_poller* self,struct candy_poller_event* ev){
	FD_SET(ev->fd,&self->rd_set);
	FD_SET(ev->fd,&self->err_set);
}
void candy_poller_unset_in(struct candy_poller* self,struct candy_poller_event* ev){
	FD_CLR(ev->fd,&self->rd_set);
	FD_CLR(ev->fd,&self->err_set);
}
void candy_poller_set_out(struct candy_poller* self,struct candy_poller_event* ev){
	FD_SET(ev->fd,&self->wr_set);
}
void candy_poller_unset_out(struct candy_poller* self,struct candy_poller_event* ev){
	FD_CLR(ev->fd,&self->wr_set);
}
int candy_poller_poll(struct candy_poller* self,int timeout){
	int sz = self->wr_set.fd_count + self->rd_set.fd_count + self->err_set.fd_count;
	if(sz <= 0 ){
		Sleep(100);
		return 0;
	}
	struct timeval tv;
	tv.tv_sec = timeout/1000;
	tv.tv_usec = ( timeout - tv.tv_sec*1000)*1000;
	fd_set wr = self->wr_set;
	fd_set rd = self->rd_set;
	fd_set err = self->err_set;
	int ret = select(0,&rd,&wr,&err,&tv);
	if(ret == 0){
		return ret;
	}
	if(ret <= 0){
		CANDY_ERROR("SELECT ERROR %d %s",CANDY_GET_ERRNO,strerror(CANDY_GET_ERRNO));
		return ret;
	}
	int size = candy_array_size(&self->arr);
	struct candy_poller_event* ev;
	self->cur = 0;
	while(self->cur<size){
		candy_array_at(&self->arr,self->cur,&ev);
		ev->revent = 0;
		if(FD_ISSET(ev->fd,&rd) || FD_ISSET(ev->fd,&err)){
			ev->revent |= CANDY_POLLER_IN;
		}
		if(FD_ISSET(ev->fd,&wr)){
			ev->revent |= CANDY_POLLER_OUT;
		}
		candy_poller_event_execute(ev);
		self->cur ++;
		size = candy_array_size(&self->arr);
	}
	return ret;
}
