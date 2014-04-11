#ifndef CANDY_H_INCLUDE_
#define CANDY_H_INCLUDE_
#ifdef __cplusplus
extern "C" {
#endif

#define CANDY_HAUSNUMERO 156384712

#define CANDY_EUNSATRT (CANDY_HAUSNUMERO+1)

#define CANDY_EFULL (CANDY_HAUSNUMERO+2)

#define CANDY_ENOTAIO (CANDY_HAUSNUMERO+3)

struct candy_callback{
	void (*connect_fn)(void* arg);
	void (*message_fn)(void* arg,void* buf,int len);
	void (*drain_fn)(void* arg);
	void (*close_fn)(void* arg,int code);
	void (*accept_fn)(void* arg,int s);
	void *arg;
};
int candy_start(int nt);
int candy_stop();

int candy_socket();
int candy_set_callback(int s,struct candy_callback cb);
int candy_connect(int s,const char* ip,int port,int timeout);
int candy_listen(int s,const char* ip,int port);
int candy_send(int s,void* buf,int len);
int candy_close(int s);

int candy_set_recvbuf_size(int s,int size);
int candy_set_sendbuf_size(int s,int size);
int candy_set_nodelay(int s,int flag);
#ifdef __cplusplus
}
#endif
#endif
