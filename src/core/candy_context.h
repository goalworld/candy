#ifndef CANDY_CONTEXT_H_
#define CANDY_CONTEXT_H_

#include "../aio/candy_worker_pool.h"
#include "../aio/candy_aio_pool.h"
struct candy_aio;
struct candy_worker;

struct candy_context{
	struct candy_aio_pool aio_pool;
	struct candy_worker_pool worker_pool;
};

void candy_context_init(struct candy_context* self,int num_thread,int max_aio);
void candy_context_destroy(struct candy_context* self);
struct candy_aio* candy_context_get_aio(struct candy_context* self,int s);
int candy_context_create_aio(struct candy_context* self);
int candy_context_destroy_aio(struct candy_context* self,int s);

#endif

