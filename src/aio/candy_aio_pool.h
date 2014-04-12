#ifndef CANDY_AIO_POOL_H_
#define CANDY_AIO_POOL_H_

#include "../base/candy_thread.h"

#define CANDY_AIO_POOL_INVILD_HANDLE -1
struct candy_aio;
struct candy_worker;
struct candy_aio_pool{
	struct candy_mutex mtx;
	struct candy_aio** aio_map;
	int* unused;
	int max_aio;
	int num_aio;

};
void candy_aio_pool_init(struct candy_aio_pool* self,int max_aio);
void candy_aio_pool_destroy(struct candy_aio_pool* self);
struct candy_aio* candy_aio_pool_get_aio(struct candy_aio_pool* self,int handle);
struct candy_aio* candy_aio_pool_alloc_aio(struct candy_aio_pool* self,struct candy_worker* worker);
int candy_aio_pool_free(struct candy_aio_pool* self, int handle);

#endif
