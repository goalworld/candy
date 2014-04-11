#ifndef CANDY_POLLER_H_
#define CANDY_POLLER_H_

#include "candy_socket.h"

#define CANDY_POLLER_IN 1
#define CANDY_POLLER_OUT 2

#if defined CANDY_HAVE_WINDOWS
#include "candy_poller_select.h"
#elif defined CANDY_USE_POLL
#include "candy_poller_poll.h"
#elif defined CANDY_USE_EPOLL
#include "candy_poller_epoll.h"
#elif defined CANDY_USE_KQUEUE
#include "candy_poller_kqueue.h"
#endif

typedef void (*candy_poller_event_fn)(void *arg ,int event);

struct candy_poller_event{
	struct candy_poller* poller;
	candy_poller_event_fn fn;
	void *arg;
	int revent;
	int event;
	candy_socket_t fd;
};
void candy_poller_event_init(struct candy_poller_event* ptr,candy_socket_t fd,candy_poller_event_fn fn,void* arg);
void  candy_poller_event_destroy(struct candy_poller_event* ptr);
void candy_poller_event_set_in(struct candy_poller_event* self);
void candy_poller_event_unset_in(struct candy_poller_event* self);
int candy_poller_event_is_set_out(struct candy_poller_event* self);
void candy_poller_event_set_out(struct candy_poller_event* self);
void candy_poller_event_unset_out(struct candy_poller_event* self);
void candy_poller_event_execute(struct candy_poller_event* self);


void candy_poller_init(struct candy_poller*);
void candy_poller_destroy(struct candy_poller*);
void candy_poller_add(struct candy_poller*,struct candy_poller_event*);
void candy_poller_remove(struct candy_poller*,struct candy_poller_event*);
void candy_poller_set_in(struct candy_poller*,struct candy_poller_event*);
void candy_poller_unset_in(struct candy_poller*,struct candy_poller_event*);
void candy_poller_set_out(struct candy_poller*,struct candy_poller_event*);
void candy_poller_unset_out(struct candy_poller*,struct candy_poller_event*);
int candy_poller_poll(struct candy_poller*,int timeout);


#endif
