#ifndef CANDY_CONTEXT_H_
#define CANDY_CONTEXT_H_

struct candy_aio;
struct candy_worker;

struct candy_aio* candy_context_get_aio(int s);
struct candy_aio* candy_context_alloc_aio();
int candy_context_init(int num_thread,int max_aio);
int candy_context_destroy();
int candy_context_create_aio();
int candy_context_destroy_aio(int s);
struct candy_worker* candy_context_next_worker();

int candy_context_inited();

#endif

