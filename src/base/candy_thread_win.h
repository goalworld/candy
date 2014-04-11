
#include "candy_window.h"

#define CANDY_NULL_THREAD_ID 0
typedef DWORD thread_id;
struct candy_thread{
	candy_thread_fn fn;
	void * arg;
	HANDLE handle;
	thread_id id;
};
struct candy_mutex{
	CRITICAL_SECTION cs;
};
