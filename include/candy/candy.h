#ifndef CANDY_H_INCLUDE_
#define CANDY_H_INCLUDE_
#ifdef __cplusplus
extern "C" {
#endif

#include "./candy_export.h"

struct candy_callback{
	void (*connect_fn)(void* arg);
	void (*message_fn)(void* arg,void* buf,int len);
	void (*drain_fn)(void* arg);
	void (*close_fn)(void* arg,int code);
	void (*accept_fn)(void* arg,int s);
	void *arg;
};
CANDY_EXPORT void candy_start(int nt);
CANDY_EXPORT void candy_wait();
CANDY_EXPORT void candy_stop();

CANDY_EXPORT int candy_aio();
CANDY_EXPORT int candy_set_callback(int s,struct candy_callback cb);
CANDY_EXPORT int candy_connect(int s,const char* ip,int port,int timeout);
CANDY_EXPORT int candy_listen(int s,const char* ip,int port);
CANDY_EXPORT int candy_send(int s,void* buf,int len);
CANDY_EXPORT int candy_close(int s);

CANDY_EXPORT int candy_set_recvbuf_size(int s,int size);
CANDY_EXPORT int candy_set_sendbuf_size(int s,int size);
CANDY_EXPORT int candy_set_nodelay(int s,int flag);
typedef void (*candy_timer_fn)(void* arg,int handle);
CANDY_EXPORT int candy_set_timer(int timeout,int brepeat,candy_timer_fn fn,void*arg);
CANDY_EXPORT int candy_clear_timer(int id);

#ifdef __cplusplus
}
#endif

#endif

