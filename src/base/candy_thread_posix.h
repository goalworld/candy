
#include <pthread.h>
#define CANDY_NULL_THREAD_ID 0
typedef pthread_t thread_id;
struct candy_thread{
	candy_thread_fn fn;
	void * arg;
	pthread_t pid;
};
struct candy_mutex{
	pthread_mutex_t mtx;
};
