#ifndef CANDY_HTTP_SERVER_H_
#define CANDY_HTTP_SERVER_H_

struct candy_http_request;
struct candy_http_responce;

typedef void(*candy_http_handle_fn)(void *arg,struct candy_http_request* req,struct candy_http_responce* rsp);

struct candy_http_server{
	int s;
	candy_http_handle_fn fn;
	void* fn_arg;
};
int candy_http_server_init(struct candy_http_server* svr,candy_http_handle_fn fn,void *arg);
int candy_http_server_service(struct candy_http_server* svr,const char* ip,int port);
void candy_http_server_destroy(struct candy_http_server* svr);

#endif
