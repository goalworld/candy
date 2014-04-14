#include "./candy_worker_pool.h"
#include <stdlib.h>
#include "./candy_worker.h"

void candy_worker_pool_init(struct candy_worker_pool* pool,int num){
	int i;
	if(num < 0) num = 2;
	pool->num_work = num;
	pool->cur_work = 0;
	pool->workers = (struct candy_worker*)malloc(sizeof(struct candy_worker)*pool->num_work);
	for(i=0;i<pool->num_work;i++){
		candy_worker_init(&pool->workers[i],pool);
	}
}
struct candy_worker* candy_worker_pool_next(struct candy_worker_pool* pool){
	struct candy_worker* worker = &pool->workers[pool->cur_work];
	pool->cur_work = (pool->cur_work+1)%pool->num_work;
	return worker;
}
void candy_worker_pool_destroy(struct candy_worker_pool* pool){
	int i;
	for(i=0;i<pool->num_work;i++){
		candy_worker_destroy(&pool->workers[i]);
	}
	free(pool->workers );
}

