#include "candy_context.h"
#include "candy_net.h"
#include "candy_aio.h"
#include "./base/candy_glock.h"
#include <assert.h>
struct candy_context ctx;
int g_inited = 0;

#define ASSERT_STARTED assert(g_inited);

void candy_start(int num_thread){
	candy_glock_lock();
	if(g_inited) return;
	g_inited = 1;
	#if defined CANDY_HAVE_WINDOWS
		WSADATA wsa;	
		WSAStartup(MAKEWORD(2,2),&wsa);
	#endif
	candy_context_init(&ctx,num_thread,10240);
	candy_glock_unlock();
}
void candy_stop(){
	candy_glock_lock();
	if(!g_inited) return;
	g_inited = 0;
	#if defined CANDY_HAVE_WINDOWS
		WSACleanup();
	#endif
	candy_context_destroy(&ctx);
	candy_glock_unlock();
}
int candy_aio(){
	ASSERT_STARTED
	return candy_context_create_aio(&ctx);
}
int candy_set_callback(int s,struct candy_callback cb){
	ASSERT_STARTED
	struct candy_aio* aio = candy_context_get_aio(&ctx,s);
	if(!aio){
		return -CANDY_ENOTAIO; 
	}
	candy_aio_set_callback(aio,cb);
	return 0;
}
int candy_connect(int s,const char* ip,int port,int timeout){
	ASSERT_STARTED
	struct candy_aio* aio = candy_context_get_aio(&ctx,s);
	if(!aio){
		return -CANDY_ENOTAIO; 
	}
	return candy_aio_connect(aio,ip,port,timeout);
}
int candy_listen(int s,const char* ip,int port){
	ASSERT_STARTED
	struct candy_aio* aio = candy_context_get_aio(&ctx,s);
	if(!aio){
		return -CANDY_ENOTAIO; 
	}
	return candy_aio_listen(aio,ip,port);
}
int candy_send(int s,void* buf,int len){
	ASSERT_STARTED
	struct candy_aio* aio = candy_context_get_aio(&ctx,s);
	if(!aio){
		return -CANDY_ENOTAIO; 
	}
	return candy_aio_send(aio,buf,len);
}
int candy_close(int s){
	ASSERT_STARTED
	return candy_context_destroy_aio(&ctx,s);
}

int candy_set_recvbuf_size(int s,int size){
	struct candy_aio* aio = candy_context_get_aio(&ctx,s);
	if(!aio){
		return -CANDY_ENOTAIO; 
	}
	return candy_aio_set_recvbuf_size(aio,size);
}
int candy_set_sendbuf_size(int s,int size){
	struct candy_aio* aio = candy_context_get_aio(&ctx,s);
	if(!aio){
		return -CANDY_ENOTAIO; 
	}
	return candy_aio_set_sendbuf_size(aio,size);
}
int candy_set_nodelay(int s,int flag){
	struct candy_aio* aio = candy_context_get_aio(&ctx,s);
	if(!aio){
		return -CANDY_ENOTAIO; 
	}
	return candy_aio_set_nodelay(aio,flag);
}

