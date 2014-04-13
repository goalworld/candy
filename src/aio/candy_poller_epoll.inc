
#include "candy_poller.h"
#include "../base/candy_log.h"
#include "../base/candy_error.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>

void candy_poller_init(struct candy_poller* self){
	self->efd = epoll_create(10240);
	memset(self->revent,0,sizeof(self->revent));
}
void candy_poller_destroy(struct candy_poller* self){
	close(self->efd);
}
void candy_poller_add(struct candy_poller* self,struct candy_poller_event* ev){
	ev->poller = self;
	struct epoll_event ep_ev;
	ep_ev.data.ptr = ev;
	int rc = epoll_ctl(self->efd,EPOLL_CTL_ADD,ev->fd,&ep_ev);
	if(rc !=  0){
		int err = errno;
		CANDY_ERROR("epoll_ctl EPOLL_CTL_ADD ERROR %d %s",err,strerror(err));
	}
}
void candy_poller_remove(struct candy_poller* self,struct candy_poller_event* ev){
	struct epoll_event ep_ev;
	ep_ev.data.ptr = ev;
	ep_ev.events = 0;
	int rc = epoll_ctl(self->efd,EPOLL_CTL_DEL,ev->fd,&ep_ev);
	if(rc !=  0){
		int err = errno;
		CANDY_ERROR("epoll_ctl EPOLL_CTL_DEL ERROR %d %s",err,strerror(err));
	}
	int i;
	for(i=0;i<CANDY_EPOLL_REVENT_SIZE;i++){
		if(self->revent[i].data.ptr == ev){
			self->revent[i].data.ptr = NULL;
		}
	}
}
void candy_poller_set_in(struct candy_poller* self,struct candy_poller_event* ev){
	struct epoll_event ep_ev;
	ep_ev.data.ptr = ev;
	ep_ev.events  = 0;
	ep_ev.events |= | POLLERR | POLLNVAL | POLLHUP;
	if(ev->event & CANDY_POLLER_OUT){
		ep_ev.events |= EPOLLOUT;
	}
	int rc = epoll_ctl(self->efd,EPOLL_CTL_MOD,ev->fd,&ep_ev);
	if(rc !=  0){
		int err = errno;
		CANDY_ERROR("epoll_ctl EPOLL_CTL_MOD in ERROR %d %s",err,strerror(err));
	}
}
void candy_poller_unset_in(struct candy_poller* self,struct candy_poller_event* ev){
	struct epoll_event ep_ev;
	ep_ev.data.ptr = ev;
	ep_ev.events  = 0;
	if(ev->event & CANDY_POLLER_OUT){
		ep_ev.events |= EPOLLOUT;
	}
	int rc = epoll_ctl(self->efd,EPOLL_CTL_MOD,ev->fd,&ep_ev);
	if(rc !=  0){
		int err = errno;
		CANDY_ERROR("epoll_ctl EPOLL_CTL_MOD unsetin ERROR %d %s",err,strerror(err));
	}
}
void candy_poller_set_out(struct candy_poller* self,struct candy_poller_event* ev){
	struct epoll_event ep_ev;
	ep_ev.data.ptr = ev;
	ep_ev.events  = 0;
	ep_ev.events |= EPOLLOUT;
	if(ev->event & CANDY_POLLER_IN){
		ep_ev.events |= EPOLLIN;
	}
	int rc = epoll_ctl(self->efd,EPOLL_CTL_MOD,ev->fd,&ep_ev);
	if(rc !=  0){
		int err = errno;
		CANDY_ERROR("epoll_ctl EPOLL_CTL_MOD setout ERROR %d %s",err,strerror(err));
	}
}
void candy_poller_unset_out(struct candy_poller* self,struct candy_poller_event* ev){
	struct epoll_event ep_ev;
	ep_ev.data.ptr = ev;
	ep_ev.events  = 0;
	if(ev->event & CANDY_POLLER_IN){
		ep_ev.events |= EPOLLIN;
	}
	int rc = epoll_ctl(self->efd,EPOLL_CTL_MOD,ev->fd,&ep_ev);
	if(rc !=  0){
		int err = errno;
		CANDY_ERROR("epoll_ctl EPOLL_CTL_MOD unsetout ERROR %d %s",err,strerror(err));
	}
}
int candy_poller_poll(struct candy_poller* self,int timeout){
	int ret;
	while(1){
		 ret = epoll_wait(self->efd,self->revent,CANDY_EPOLL_REVENT_SIZE,timeout);
		 if(ret < 0 && (errno == EINTR)) continue;
		 else break;
	}
	if(ret > 0){
		int i;
		struct epoll_event* ep_ev;
		struct candy_poller_event* ev;
		for(i=0;i<ret;i++){
			ep_ev = &self->revent[i];
			ev = (struct candy_poller_event*)ep_ev->data.ptr;
			if(!ev){
				continue;
			}
			ev->revent = ( ( ep_ev->events & (EPOLLIN | EPOLLHUP | EPOLLERR) ) > 0 ? CANDY_POLLER_IN:0 )
						|( ( ep_ev->events & (EPOLLOUT | EPOLLHUP | EPOLLERR) ) >0 ? CANDY_POLLER_OUT:0 );
			candy_poller_event_execute(ev);
		}
	}else if(ret < 0){
		int err = errno;
		CANDY_ERROR("epoll_wait ERROR %d %s",err,strerror(err));
	}
	return ret;
}
