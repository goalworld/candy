#ifndef CANDY_AIO_H_
#define CANDY_AIO_H_
#include "candy_poller.h"
#include "../base/candy_socket.h"
#include "../base/candy_timer.h"
#include "../base/candy_thread.h"
#include "candy_worker.h"
#include "../candy.h"

enum{
	CANDY_AIO_READY=1,
	CANDY_AIO_LISTENED,
	CANDY_AIO_CONNECTING,
	CANDY_AIO_CONNECTED,
	CANDY_AIO_SHUTDOWN,
	CANDY_AIO_ERRORED,
	CANDY_AIO_CLOSED,
};

struct candy_aio{
	struct candy_aio_pool* owner;
	struct candy_worker* worker;
	struct candy_poller_event event;
	candy_socket_t sock;
	int state;//CANDY_CONNECTION_CLOSED
	struct candy_mutex sync;
	struct candy_callback callback;
	int handle;
	int is_close;
	struct candy_timer_event timer_event;
	int rcv_size;
	int snd_size;
	int bdelay;
};

#define candy_aio_get_worker(aio) (aio)->worker
#define candy_aio_get_handle(aio) (aio)->handle
#define candy_aio_set_callback(aio,cb) do{ (aio)->callback = cb; }while(0)

void candy_aio_init(struct candy_aio* aio,struct candy_aio_pool* woner,int s,struct candy_worker* worker);
int candy_aio_set_socket(struct candy_aio* aio,candy_socket_t sock);
int candy_aio_listen(struct candy_aio* aio,const char* ip,int port);
int candy_aio_connect(struct candy_aio* aio,const char* ip,int port,int timeout);
int candy_aio_send(struct candy_aio* aio,void* buf,int sz);
void candy_aio_shutdown(struct candy_aio* aio);
int candy_aio_destroy(struct candy_aio* aio);
int candy_aio_set_recvbuf_size(struct candy_aio* aio,int size);
int candy_aio_set_sendbuf_size(struct candy_aio* aio,int size);
int candy_aio_set_nodelay(struct candy_aio* aio,int flag);

#endif
