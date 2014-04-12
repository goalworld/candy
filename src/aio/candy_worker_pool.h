#ifndef CANDY_WORK_POOL_H_
#define CANDY_WORK_POOL_H_

struct candy_worker;

struct candy_worker_pool{
	struct candy_worker *workers;
	int num_work;
	int cur_work;
};
void candy_worker_pool_init(struct candy_worker_pool* pool,int num);
struct candy_worker* candy_worker_pool_next(struct candy_worker_pool* pool);
void candy_worker_pool_destroy(struct candy_worker_pool* pool);

#endif
