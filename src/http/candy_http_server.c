#include "candy_http_server.h"
#include "../aio/candy_net.h"

void http_accept_fn(void* arg,int s);

int candy_http_server_init(struct candy_http_server* svr,candy_http_handle_fn fn,void *arg){
	svr->s = candy_aio();
	svr->fn = fn;
	svr->fn_arg = arg;
	struct candy_callback cb;
	cb.arg = svr;
	cb.accept_fn = http_accept_fn;
	candy_set_callback(svr->s, cb);
}
int candy_http_server_service(struct candy_http_server* svr,const char* ip,int port){
	return candy_listen(svr->s,ip,port);
}
void candy_http_server_destroy(struct candy_http_server* svr){
	candy_close(svr->s);
}

void http_accept_fn(void* arg,int s){
	
}