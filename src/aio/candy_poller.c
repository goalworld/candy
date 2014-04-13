#include "candy_poller.h"
#include "../base/candy_log.h"

void 
candy_poller_event_init(struct candy_poller_event* ptr,candy_socket_t fd,candy_poller_event_fn fn,void* arg){
	(ptr)->fd = (fd);							
	(ptr)->event = 0;							
	(ptr)->revent = 0;							
	(ptr)->fn = (fn);							
	(ptr)->arg = (arg);	
};
void  
candy_poller_event_destroy(struct candy_poller_event* ptr){
	
}

void 
candy_poller_event_set_in(struct candy_poller_event* self){
	if(self->event & CANDY_POLLER_IN){
		CANDY_INFO("candy_poller_event_set_in exist %d",self->fd);
		return;
	}
	candy_poller_set_in(self->poller,self);
	self->event |= CANDY_POLLER_IN;
}
void 
candy_poller_event_unset_in(struct candy_poller_event* self){
	if(self->event & CANDY_POLLER_IN){
		candy_poller_unset_in(self->poller,self);
		self->event &= ~CANDY_POLLER_IN;
	}
}
int 
candy_poller_event_is_set_out(struct candy_poller_event* self){
	return self->event & CANDY_POLLER_OUT;
}	
void 
candy_poller_event_set_out(struct candy_poller_event* self){
	if(self->event & CANDY_POLLER_OUT){
		CANDY_INFO("candy_poller_event_set_out exist %d",self->fd);
		return;
	}
	candy_poller_set_out(self->poller,self);
	self->event |= CANDY_POLLER_OUT;
}
void 
candy_poller_event_unset_out(struct candy_poller_event* self){
	if(self->event & CANDY_POLLER_OUT){
		candy_poller_unset_out(self->poller,self);
		self->event &= ~CANDY_POLLER_OUT;
	}
}
void 
candy_poller_event_execute(struct candy_poller_event* self){
	if(self->revent&CANDY_POLLER_IN && self->event & CANDY_POLLER_IN){
		self->fn(self->arg,CANDY_POLLER_IN);
	}
	if(self->revent&CANDY_POLLER_OUT && self->event & CANDY_POLLER_OUT){
		self->fn(self->arg,CANDY_POLLER_OUT);
	}
}


#if defined CANDY_HAVE_WINDOWS
#include "candy_poller_select.inc"
#elif defined CANDY_USE_POLL
#include "candy_poller_poll.inc"
#elif defined CANDY_USE_EPOLL
#include "candy_poller_epoll.inc"
#elif defined CANDY_USE_KQUEUE
#include "candy_poller_kqueue.inc"
#endif
