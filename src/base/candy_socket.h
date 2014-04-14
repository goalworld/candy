#ifndef CANDY_SOCKET_H_
#define CANDY_SOCKET_H_
#if defined CANDY_HAVE_WINDOWS
	#include "../base/candy_window.h"
	#define candy_socket_t SOCKET
	#define CANDY_INVALID_SOCKET INVALID_SOCKET
#else
	#include <sys/socket.h>
	#include <unistd.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <fcntl.h>
	#include <netinet/tcp.h>
	#define candy_socket_t int
	#define CANDY_INVALID_SOCKET -1
#endif

candy_socket_t candy_socket_connect_async(const char* ip,int port);
candy_socket_t candy_socket_connect_sync(const char* ip,int port);
candy_socket_t candy_socket_listen(const char* ip,int port);
candy_socket_t candy_socket_accept(candy_socket_t server);
int candy_socket_write(candy_socket_t sock,void* buf,int sz);
int candy_socket_read(candy_socket_t sock,void* buf,int sz);

#if !defined CANDY_HAVE_WINDOWS
int candy_socket_writev(candy_socket_t sock,const struct iovec *iov, int cnt);
int candy_socket_readv(candy_socket_t sock,const struct iovec *iov, int cnt);
#endif

int candy_socket_shutdown(candy_socket_t sock,int which);//SHUT_RD SHUT_WR SHUT_RDWR
int candy_socket_close(candy_socket_t sock);

int candy_socket_set_keep_alive(candy_socket_t sock,int flag);
int candy_socket_set_addr_reuse(candy_socket_t sock,int falg);
int candy_socket_set_nodelay(candy_socket_t fd,int flag);//0,1
int candy_socket_set_noblock(candy_socket_t fd,int flag);//0,1
int candy_socket_set_recvbuf_size(candy_socket_t fd,int flag);//SIZE
int candy_socket_set_sendbuf_size(candy_socket_t fd,int flag);//SIZE

#endif
