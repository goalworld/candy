#include "./candy_aio.h"
#include "./candy_aio_pool.h"
#include "candy/candy_log.h"
#include "./candy_worker.h"
#include "./candy_worker_pool.h"
#include <memory.h>


#define CANDY_AIO_READ_BUFFER_SIZE 65536
static void candy_aio_cb_accept(void *arg);
static void candy_aio_accept(struct candy_aio* aio);
static void candy_aio_read(struct candy_aio* aio);
static void candy_aio_regist_event(void* arg);
static void candy_aio_event(void* arg,int event);
static void candy_aio_handle_error(struct candy_aio* aio,int code);
static void candy_aio_timer(void *arg);
void candy_aio_init(struct candy_aio* aio,struct candy_aio_pool* woner,int s,struct candy_worker* worker){
	aio->state = CANDY_AIO_READY;
	aio->worker = worker;
	aio->owner = woner;
	aio->sock = CANDY_INVALID_SOCKET;
	aio->handle = s;
	aio->is_close = 0;
	aio->snd_size = 0;
	aio->rcv_size = 0;
	aio->bdelay = 0;
	//memset(&aio->callback,0,sizeof(aio->callback));
	candy_mutex_init(&aio->sync);
}
int candy_aio_set_socket(struct candy_aio* aio,candy_socket_t sock){
	candy_mutex_lock(aio->sync);
	if(aio->state != CANDY_AIO_READY){
		candy_mutex_unlock(aio->sync);
		return -1;
	}
	aio->sock = sock;
	aio->state = CANDY_AIO_CONNECTED;
	candy_socket_set_noblock(sock,1);
	candy_poller_event_init(&aio->event,sock,candy_aio_event,aio);
	candy_worker_execute(aio->worker,candy_aio_regist_event,aio);
	candy_mutex_unlock(aio->sync);
	return 0;
}
int candy_aio_listen(struct candy_aio* aio,const char* ip,int port){
	candy_mutex_lock(aio->sync);
	if(aio->state != CANDY_AIO_READY){
		candy_mutex_unlock(aio->sync);
		return -1;
	}
	candy_socket_t sock;
	sock = candy_socket_listen(ip,port);
	if(sock == CANDY_INVALID_SOCKET){
		candy_mutex_unlock(aio->sync);
		return -1;
	}
	candy_socket_set_noblock(sock,1);
	aio->sock = sock;
	aio->state = CANDY_AIO_LISTENED;
	if(aio->rcv_size > 0){
		candy_socket_set_recvbuf_size(aio->sock, aio->rcv_size);
	}
	if(aio->snd_size > 0){
		candy_socket_set_sendbuf_size(aio->sock, aio->snd_size);
	}
	if(aio->bdelay > 0){
		candy_socket_set_nodelay(aio->sock, aio->bdelay);
	}
	candy_poller_event_init(&aio->event,sock,candy_aio_event,aio);
	candy_worker_execute(aio->worker,candy_aio_regist_event,aio);
	candy_mutex_unlock(aio->sync);
	return 0;
	
}
int candy_aio_connect(struct candy_aio* aio,const char* ip,int port,int timeout){
	candy_mutex_lock(aio->sync);
	if(aio->state != CANDY_AIO_READY){
		candy_mutex_unlock(aio->sync);
		return -1;
	}
	candy_socket_t sock;
	sock = candy_socket_connect_async(ip,port);
	if(sock == CANDY_INVALID_SOCKET){
		candy_mutex_unlock(aio->sync);
		return -1;
	}
	aio->sock = sock;
	aio->state = CANDY_AIO_CONNECTING;
	if(aio->rcv_size > 0){
		candy_socket_set_recvbuf_size(aio->sock, aio->rcv_size);
	}
	if(aio->snd_size > 0){
		candy_socket_set_sendbuf_size(aio->sock, aio->snd_size);
	}
	if(aio->bdelay > 0){
		candy_socket_set_nodelay(aio->sock, aio->bdelay);
	}
	candy_timer_event_init(&aio->timer_event,timeout,0,candy_aio_timer,aio);
	candy_poller_event_init(&aio->event,sock,candy_aio_event,aio);
	candy_worker_execute(aio->worker,candy_aio_regist_event,aio);
	candy_mutex_unlock(aio->sync);
	return 0;
}
void candy_aio_timer(void *arg){
	struct candy_aio* aio = (struct candy_aio*)arg;
	if(aio->state == CANDY_AIO_CONNECTING){
		candy_aio_handle_error(aio, CANDY_ETIMEOUT);
	}else{
		CANDY_ERROR("candy_aio_timer bad state : %d",aio->state);
	}
}
int candy_aio_send(struct candy_aio* aio,void* buf,int sz){
	if(aio->state == CANDY_AIO_CONNECTED){
		int rc = candy_socket_write(aio->sock,buf,sz);
		if(rc >= 0&& rc < sz){
			if(!candy_poller_event_is_set_out(&aio->event))
				candy_worker_execute(aio->worker,(candy_worker_fn)candy_poller_event_set_out,(void*)&aio->event);
		}
		return rc;
	}
	return -1;
}
void candy_aio_shutdown(struct candy_aio* aio){
	candy_mutex_lock(aio->sync);
	if(aio->state == CANDY_AIO_CONNECTED){
		candy_socket_shutdown(aio->sock,CANDY_SHUTDOWN_RDWR);
		aio->state = CANDY_AIO_SHUTDOWN;
	}
	candy_mutex_unlock(aio->sync);
}
int candy_aio_destroy(struct candy_aio* aio){
	CANDY_CHECK(candy_worker_in_loop(aio->worker) == 0);
	candy_mutex_lock(aio->sync);
	if(aio->state != CANDY_AIO_CLOSED && aio->state != CANDY_AIO_READY){
		aio->state = CANDY_AIO_CLOSED;
		candy_worker_remove_timer(aio->worker,&aio->timer_event);
		candy_worker_remove_event(aio->worker,&aio->event);
		candy_poller_event_destroy(&aio->event);
		candy_socket_close(aio->sock);
	}
	candy_mutex_unlock(aio->sync);
	candy_mutex_destroy(aio->sync);
	return 0;
}
void candy_aio_cb_accept(void *arg){
	struct candy_aio* aio = (struct candy_aio*)arg;
	if(aio->callback.accept_fn){
		aio->callback.accept_fn(aio->callback.arg,aio->handle);
	}
}
void candy_aio_accept(struct candy_aio* aio){
	candy_socket_t sock = candy_socket_accept(aio->sock);
	struct candy_worker * next = candy_worker_pool_next(candy_worker_owner(aio->worker));
	struct candy_aio* tmp = candy_aio_pool_alloc_aio(aio->owner,next);
	if(!tmp){
		CANDY_ERROR("candy_aio_accept FD %s","full");
		candy_socket_close(sock);
	}else{
		tmp->callback.accept_fn = aio->callback.accept_fn;
		tmp->callback.arg = aio->callback.arg;
		candy_aio_set_socket(tmp,sock);
		candy_worker_execute(tmp->worker,candy_aio_cb_accept,tmp);
	}
}
void candy_aio_regist_event(void* arg){
	struct candy_aio* aio = (struct candy_aio*)arg;
	if(aio->state == CANDY_AIO_LISTENED){
		candy_worker_add_event(aio->worker,&aio->event);
		candy_poller_event_set_in(&aio->event);
	}else if(aio->state == CANDY_AIO_CONNECTING){
		candy_worker_add_event(aio->worker,&aio->event);
		candy_poller_event_set_out(&aio->event);
		candy_worker_add_timer(aio->worker,&aio->timer_event);
	}else if(aio->state == CANDY_AIO_CONNECTED){
		candy_worker_add_event(aio->worker,&aio->event);
		candy_poller_event_set_in(&aio->event);
	}
}

void candy_aio_event(void* arg,int event){
	struct candy_aio* aio = (struct candy_aio*)arg;
	if(event&CANDY_POLLER_IN){
		if(aio->state == CANDY_AIO_LISTENED){
			candy_aio_accept(aio);
		}else if(aio->state == CANDY_AIO_CONNECTED){
			candy_aio_read(aio);
		}else{
			candy_poller_event_unset_in(&aio->event);
			CANDY_ERROR("candy_aio_event IN bad state : %d",aio->state);
		}
	}
	if(event&CANDY_POLLER_OUT){
		candy_poller_event_unset_out(&aio->event);
		if(aio->state == CANDY_AIO_CONNECTING){
			candy_worker_remove_timer(aio->worker,&aio->timer_event);
			candy_poller_event_set_in(&aio->event);
			aio->state = CANDY_AIO_CONNECTED;
			if(aio->callback.connect_fn){
				aio->callback.connect_fn(aio->callback.arg);
			}	
		}else if(aio->state == CANDY_AIO_CONNECTED){
			if(aio->callback.drain_fn){
				aio->callback.drain_fn(aio->callback.arg);
			}
		}else{
			CANDY_ERROR("candy_aio_event OUT bad state : %d",aio->state);
		}
	}
}

void candy_aio_read(struct candy_aio* aio){
	char buf[CANDY_AIO_READ_BUFFER_SIZE];
	int rc;
	rc = candy_socket_read(aio->sock,buf,CANDY_AIO_READ_BUFFER_SIZE);
	if(rc > 0){
		if(aio->callback.message_fn){
			aio->callback.message_fn(aio->callback.arg,buf,rc);
		}
		return;
	}else if(rc == 0){
		int err = CANDY_GET_ERRNO;
		if( err == CANDY_EAGAIN)
			return;
		candy_aio_handle_error(aio,0);
	}else{
		int err = CANDY_GET_ERRNO;
		candy_aio_handle_error(aio,err);
	}
}
void candy_aio_handle_error(struct candy_aio* aio,int code){
	candy_poller_event_unset_out(&aio->event);
	candy_poller_event_unset_in(&aio->event);
	if(aio->state == CANDY_AIO_CONNECTED){
		candy_socket_shutdown(aio->sock,CANDY_SHUTDOWN_RDWR);
	}
	aio->state = CANDY_AIO_ERRORED;
	if(aio->callback.close_fn){
		aio->callback.close_fn(aio->callback.arg,code);
	}
}

int candy_aio_set_recvbuf_size(struct candy_aio* aio,int size){
	if(size < 0) return-1;
	if(	aio->state == CANDY_AIO_READY){
		aio->rcv_size = size;
		return 0;
	}else if(aio->state != CANDY_AIO_ERRORED && aio->state != CANDY_AIO_CLOSED){
		if(aio->rcv_size != size){
			aio->rcv_size = size;
			return candy_socket_set_recvbuf_size(aio->sock, size);
		}else{
			return 0;
		}
	}
	return -1;
}
int candy_aio_set_sendbuf_size(struct candy_aio* aio,int size){
	if(size < 0) return-1;
	if(	aio->state == CANDY_AIO_READY){
		aio->snd_size = size;
	}else if(aio->state != CANDY_AIO_ERRORED && aio->state != CANDY_AIO_CLOSED){
		if(aio->snd_size != size){
			aio->snd_size = size;
			candy_socket_set_sendbuf_size(aio->sock, size);
		}else{
			return 0;
		}
	}
	return -1;
}
int candy_aio_set_nodelay(struct candy_aio* aio,int flag){
	if(	aio->state == CANDY_AIO_READY){
		aio->bdelay = flag;
	}else if(aio->state != CANDY_AIO_ERRORED && aio->state != CANDY_AIO_CLOSED){
		if(aio->bdelay != flag){
			aio->bdelay = flag;
			candy_socket_set_nodelay(aio->sock, flag);
		}else{
			return 0;
		}
	}
	return -1;
}
