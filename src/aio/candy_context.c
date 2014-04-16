
#include "candy/candy_log.h"
#include "candy/candy_error.h"
#include "candy_context.h"

#include "candy_aio.h"
#include "candy_worker.h"

#include <stdlib.h>
#include <memory.h>

void 
candy_context_init(struct candy_context* self,int num_thread,int max_aio){
	candy_aio_pool_init(&self->aio_pool,max_aio);
	candy_worker_pool_init(&self->worker_pool,num_thread);
}
void 
candy_context_destroy(struct candy_context* self){
	candy_aio_pool_destroy(&self->aio_pool);
	candy_worker_pool_destroy(&self->worker_pool);
}

int 
candy_context_create_aio(struct candy_context* self){
	struct candy_aio* aio = candy_aio_pool_alloc_aio(&self->aio_pool,candy_worker_pool_next(&self->worker_pool));
	if(aio){
		return candy_aio_get_handle(aio);
	}
	return CANDY_AIO_POOL_INVILD_HANDLE; 
}

struct candy_aio* 
candy_context_get_aio(struct candy_context* self,int s){
	return candy_aio_pool_get_aio(&self->aio_pool,s);
}

int 
candy_context_destroy_aio(struct candy_context* self,int s){
	return candy_aio_pool_free(&self->aio_pool,s);
}
