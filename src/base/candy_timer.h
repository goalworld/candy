#ifndef CANDY_TIMER_H_
#define CANDY_TIMER_H_

#include <time.h>
#include "candy_array.h"

typedef void(*candy_timer_fn)(void* arg);

struct candy_timer_event{
	int timeout;
	int brepeat;
	candy_timer_fn fn;
	void* arg;
	clock_t pre;
};
void candy_timer_event_init(struct candy_timer_event*ptr,int timeout,int brepeat,candy_timer_fn fn,void *arg);
struct candy_timer{
	struct candy_array arr;
	int cur;
};
void candy_timer_init(struct candy_timer* timer);
void candy_timer_destroy(struct candy_timer* timer);
int candy_timer_execute(struct candy_timer* timer);
void candy_timer_add(struct candy_timer* timer,struct candy_timer_event* ev);
void candy_timer_remove(struct candy_timer* timer,struct candy_timer_event* ev);

#endif
