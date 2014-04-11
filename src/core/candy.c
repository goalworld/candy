#include "candy_context.h"
#include "../candy.h"
#include "../aio/candy_aio.h"


int candy_start(int num_thread){
	if(num_thread <=0) return -1;
	int rc;
	rc = candy_context_init(num_thread,10240);
	return rc;
}
int candy_stop(){
	int rc;
	rc = candy_context_destroy();
	return rc;
}
int candy_socket(){
	if(candy_context_inited() != 0){
		return -CANDY_EUNSATRT;
	}
	int s = candy_context_create_aio();
	return s;
}
int candy_set_callback(int s,struct candy_callback cb){
	
	struct candy_aio* aio = candy_context_get_aio(s);
	if(!aio){
		return -CANDY_ENOTAIO; 
	}
	candy_aio_set_callback(aio,cb);
	return 0;
}
int candy_connect(int s,const char* ip,int port,int timeout){
	struct candy_aio* aio = candy_context_get_aio(s);
	if(!aio){
		return -CANDY_ENOTAIO; 
	}
	return candy_aio_connect(aio,ip,port,timeout);
}
int candy_listen(int s,const char* ip,int port){
	struct candy_aio* aio = candy_context_get_aio(s);
	if(!aio){
		return -CANDY_ENOTAIO; 
	}
	return candy_aio_listen(aio,ip,port);
}
int candy_send(int s,void* buf,int len){
	struct candy_aio* aio = candy_context_get_aio(s);
	if(!aio){
		return -CANDY_ENOTAIO; 
	}
	return candy_aio_send(aio,buf,len);
}
int candy_close(int s){
	int rc;
	rc = candy_context_destroy_aio(s);
	return rc;
}

int candy_set_recvbuf_size(int s,int size){
	struct candy_aio* aio = candy_context_get_aio(s);
	if(!aio){
		return -CANDY_ENOTAIO; 
	}
	return candy_aio_set_recvbuf_size(aio,size);
}
int candy_set_sendbuf_size(int s,int size){
	struct candy_aio* aio = candy_context_get_aio(s);
	if(!aio){
		return -CANDY_ENOTAIO; 
	}
	return candy_aio_set_sendbuf_size(aio,size);
}
int candy_set_nodelay(int s,int flag){
	struct candy_aio* aio = candy_context_get_aio(s);
	if(!aio){
		return -CANDY_ENOTAIO; 
	}
	return candy_aio_set_nodelay(aio,flag);
}

