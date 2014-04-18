#ifndef CANDY_TIMER_H_
#define CANDY_TIMER_H_

#include <time.h>
#include "candy/candy_array.h"

typedef void(*candy_timerset_fn)(void* arg,int id);
struct candy_worker;
struct candy_timerset_event{
	int timeout;
	int brepeat;
	candy_timerset_fn fn;
	void* arg;
	clock_t pre;
	int bremove;
	int handle;
};
void candy_timerset_event_init(struct candy_timerset_event*ptr,int timeout,int brepeat,candy_timerset_fn fn,void *arg,int handle);
#define candy_timerset_event_remove(ptr)(ptr)->bremove = 1
struct candy_timerset{
	struct candy_array arr;
	int cur;
};
void candy_timerset_init(struct candy_timerset* timer);
void candy_timerset_destroy(struct candy_timerset* timer);
int candy_timerset_execute(struct candy_timerset* timer);
void candy_timerset_add(struct candy_timerset* timer,struct candy_timerset_event* ev);
void candy_timerset_remove(struct candy_timerset* timer,struct candy_timerset_event* ev);

#endif
