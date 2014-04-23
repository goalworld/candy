#include "candy_timerset.h"
#include "candy/candy_log.h"
#include "candy/candy_sleep.h"
void 
candy_timerset_event_init(struct candy_timerset_event*ptr,int timeout,int brepeat,candy_timerset_fn fn,void *arg,int handle){
	(ptr)->timeout = (timeout);					
	(ptr)->brepeat = brepeat;					
	(ptr)->fn = (fn);							
	(ptr)->arg = (arg);
	ptr->handle = handle;
	ptr->bremove = 0;
}

void candy_timerset_init(struct candy_timerset* timer){
	candy_array_init(&timer->arr,sizeof(void*));
}
void candy_timerset_destroy(struct candy_timerset* timer){
	candy_array_destroy(&timer->arr);
}
int candy_timerset_execute(struct candy_timerset* timer){
	int size = candy_array_size(&timer->arr);

	struct candy_timerset_event* ev;
	timer->cur = 0;
	int min = 500;
	while(timer->cur<size){
		int now = candy_time_now();
		candy_array_at(&timer->arr,timer->cur,&ev);
		if(ev->bremove != 0){
			candy_array_earse(&timer->arr,timer->cur,1,NULL);
			continue;
		}
		int df = now - ev->pre - ev->timeout;
		CANDY_INFO("%d %d %d %d",size,df,now,ev->pre);
		if(df >= 0 ){
			if(!ev->brepeat){
				candy_array_earse(&timer->arr,timer->cur,1,NULL);
				timer->cur--;
			}
			ev->fn(ev->arg,ev->handle);
			ev->pre = candy_time_now();
		}else{
			if(min > -df){
				min = -df;
			}
		}
		timer->cur++;
		size = candy_array_size(&timer->arr);
	}
	CANDY_INFO("%s","break");
	return min;
}
void candy_timerset_add(struct candy_timerset* timer,struct candy_timerset_event* ev){
	ev->pre = candy_time_now();
	candy_array_push(&timer->arr,&ev);
}
void candy_timerset_remove(struct candy_timerset* timer,struct candy_timerset_event* ev){
	int size = candy_array_size(&timer->arr);
	struct candy_timerset_event* cur;
	int i = 0;
	while(i<size){
		candy_array_at(&timer->arr,i,&cur);
		if(cur == ev){
			candy_array_earse(&timer->arr,i,1,NULL);
			if(i <= timer->cur && timer->cur != 0){
				timer->cur--;
			}
			return;
		}
		i++;
	}
}
