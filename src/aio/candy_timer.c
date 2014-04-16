#include "candy_timer.h"
#include "candy/candy_log.h"

void 
candy_timer_event_init(struct candy_timer_event*ptr,int timeout,int brepeat,candy_timer_fn fn,void *arg){										
	(ptr)->timeout = (timeout);					
	(ptr)->brepeat = brepeat;					
	(ptr)->fn = (fn);							
	(ptr)->arg = (arg);							
}

void candy_timer_init(struct candy_timer* timer){
	candy_array_init(&timer->arr,sizeof(void*));
}
void candy_timer_destroy(struct candy_timer* timer){
	candy_array_destroy(&timer->arr);
}
int candy_timer_execute(struct candy_timer* timer){
	int size = candy_array_size(&timer->arr);
	struct candy_timer_event* ev;
	timer->cur = 0;
	clock_t min = 500;
	while(timer->cur<size){
		clock_t now = clock();
		candy_array_at(&timer->arr,timer->cur,&ev);
		clock_t df = now - ev->pre - ev->timeout;
		if(df >= 0 ){
			if(!ev->brepeat){
				candy_array_earse(&timer->arr,timer->cur,1,NULL);
				timer->cur--;
			}
			ev->fn(ev->arg);
		}else{
			if(min > df){
				min = df;
			}
		}
		timer->cur++;
		size = candy_array_size(&timer->arr);
	}
	return min;
}
void candy_timer_add(struct candy_timer* timer,struct candy_timer_event* ev){
	ev->pre = clock();
	candy_array_push(&timer->arr,&ev);
}
void candy_timer_remove(struct candy_timer* timer,struct candy_timer_event* ev){
	int size = candy_array_size(&timer->arr);
	struct candy_timer_event* cur;
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
