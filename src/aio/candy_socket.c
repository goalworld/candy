#include "./candy_socket.h"
#include <string.h>
#include "../base/candy_error.h"
#include "../base/candy_log.h"
static int 
candy_check_ip(const char* host){
	unsigned int i=0,count = 0;
	size_t len = strlen(host);
	for ( ; i < len ; i++ ) {
		if ( !(host[i] >= '0' && host[i] <= '9' )) {
			if ( host[i] == '.' ) {
				count++;
				continue;
			}
			return -1;
		}
	}
	return  (count == 3 )?0:-1;
}
static int 
candy_parse_addr(const char* host,struct in_addr * addr){
	if( candy_check_ip(host) == 0 ){
		addr->s_addr = inet_addr((char *)( host));
	}else{
		struct hostent* htt = gethostbyname(host);
		if(htt->h_length <= 0){
			CANDY_WARN("unknow host:%s %s",host);
			return -1;
		}else{
			*addr = *( (struct in_addr *)htt->h_addr_list[0] );
		}
	}
	return 0;
	
}
static int 
candy_do_connect(candy_socket_t sock ,const char* host, int port){
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons((unsigned short)(port));
	if(candy_parse_addr(host,&addr.sin_addr) != 0){
		return -1 ;
	}
	int ret = connect(sock, (struct sockaddr *)(&addr), sizeof(addr));
	if(ret < 0){
		int err = CANDY_GET_ERRNO;
		if((err == CANDY_EINPROGRESS) || (err == CANDY_EAGAIN)){
			return 0;
		}else{
			CANDY_WARN("%s:%d CONNECT ERROR %s",host,port,strerror(err));
			return -1;
		}
	}
	return 0;
}

candy_socket_t candy_socket_connect_async(const char* ip,int port){
	candy_socket_t sock = socket(AF_INET, SOCK_STREAM,0);
	if(sock < 0){
	 	CANDY_WARN(" %s:%d socket create failed%d",ip,port,CANDY_GET_ERRNO);
		return CANDY_INVALID_SOCKET;
	 }
	candy_socket_set_noblock(sock,1);
	if( candy_do_connect(sock,ip,port) != 0){
		candy_socket_close(sock);
		return CANDY_INVALID_SOCKET;
	}
	return sock;
}
candy_socket_t candy_socket_connect_sync(const char* ip,int port){
	candy_socket_t sock = socket(AF_INET, SOCK_STREAM,0);
	if(sock < 0){
	 	CANDY_WARN(" %s:%d socket create failed%d",ip,port,CANDY_GET_ERRNO);
		return CANDY_INVALID_SOCKET;
	}
	if( candy_do_connect(sock,ip,port) != 0){
		candy_socket_close(sock);
		return CANDY_INVALID_SOCKET;
	}
	return sock;
}
candy_socket_t candy_socket_listen(const char* ip,int port){
	candy_socket_t sock = socket(AF_INET,SOCK_STREAM,0);
	 if(sock < 0){
	 	CANDY_WARN(" %s:%d socket create failed%d",ip,port,CANDY_GET_ERRNO);
		return CANDY_INVALID_SOCKET;
	 }
	 struct sockaddr_in addr;
	 memset(&addr, 0, sizeof(addr));
	 addr.sin_family = AF_INET;
	 addr.sin_port = htons((unsigned short)(port));
	 if(candy_parse_addr(ip,&addr.sin_addr) != 0){
		 candy_socket_close(sock);
		 return CANDY_INVALID_SOCKET;
	 }
	 if ( bind(sock, (const struct sockaddr *)(&addr), sizeof(addr)) < 0){
	 	CANDY_WARN("bind failed %s:%d",ip,port);
		candy_socket_close(sock);
		return CANDY_INVALID_SOCKET;
	 }
	 if ( listen(sock, 512) < 0){
	 	CANDY_WARN("listen failed %s:%d",ip,port);
		candy_socket_close(sock);
		return CANDY_INVALID_SOCKET;
	 }
	 return sock;
}
candy_socket_t candy_socket_accept(candy_socket_t sock){
	while(1){
		candy_socket_t retsock =  accept(sock,NULL,NULL);
		if ( retsock < 0){
			int err = CANDY_GET_ERRNO;
			if(err == CANDY_EAGAIN){
				continue;
			}else{
				return CANDY_INVALID_SOCKET;
			}
		}
		return retsock;
	}
	return -1;
}
int candy_socket_write(candy_socket_t sock,void* inbuf,int sz){
	int ret;
	int rsz = 0;
	char * buf = (char *)inbuf;
	while(1){
		ret = send(sock,buf,sz,0);
		if(ret < 0){
			int err = CANDY_GET_ERRNO;
			if( err == CANDY_EAGAIN){
				break;
			}else if(err == CANDY_EINTR){
				continue;
			}else{
				if(rsz == 0){
					rsz = ret;
				}
				break;
			}
		}else if(ret == 0){
			break;
		}
		rsz += ret;
		buf = (char *)buf + ret;
		sz  = sz - ret;
		if(rsz == sz){
			break;
		}
	}
	return rsz;
}
int candy_socket_read(candy_socket_t sock,void* inbuf,int sz){
	int ret;
	int wsz = 0;
	char * buf = (char *)inbuf;
	while(1){
		ret = recv(sock,buf,sz,0);
		if(ret < 0){
			int err = CANDY_GET_ERRNO;
			if( err == CANDY_EAGAIN){
				break;
			}else if(err ==CANDY_EINTR){
				continue;
			}else{
				if(wsz == 0){
					wsz = ret;
					break;
				}
			}
		}else if(ret == 0){
			break;
		}
		wsz+=ret;
		buf = (char *)buf + ret;
		sz  = sz - ret;
		if(wsz == sz){
			break;
		}
	}
	return wsz;
}
#if !defined CANDY_HAVE_WINDOWS
/*
struct iovec {
char *iov_base;
size_t iov_len;
};
*/
int candy_socket_writev(candy_socket_t sock,const struct iovec *iov, int cnt){
	return -1;
}
int candy_socket_readv(candy_socket_t sock,const struct iovec *iov, int cnt){
	return -1;
}
#endif
int candy_socket_shutdown(candy_socket_t sock,int which){
	return shutdown(sock, which);
}//SHUT_RD SHUT_WR SHUT_RDWR
int candy_socket_close(candy_socket_t sock){
	#if defined CANDY_HAVE_WINDOWS
		return closesocket(sock);
	#else
		return close(sock);
	#endif
}

int candy_socket_set_keep_alive(candy_socket_t sock,int flag){
	char mode = flag?1:0;
	return setsockopt( sock, SOL_SOCKET, SO_KEEPALIVE, (const char*)&mode, sizeof(char));
}

int candy_socket_set_addr_reuse(candy_socket_t sock,int falg){
	return -1;
}
int candy_socket_set_nodelay(candy_socket_t sock,int flag){
	char mode = flag?1:0;
	return setsockopt( sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&mode, sizeof(char));
}//0,1
int candy_socket_set_noblock(candy_socket_t sock,int flag){//0,1
#if defined CANDY_HAVE_WINDOWS
	u_long mode = flag?1:0;
	return ioctlsocket(sock,FIONBIO,&mode);
#else
	int oflag = fcntl(sock, F_GETFL, 0);
	if(flag){
		oflag |= O_NONBLOCK;
	}else{
		oflag &= (~O_NONBLOCK);
	}

	if( fcntl(sock, F_SETFL, oflag) < 0){
		return -1;
	}
	return 0;
#endif
}
int candy_socket_set_recvbuf_size(candy_socket_t sock,int flag){
	return setsockopt( sock, SOL_SOCKET, SO_RCVBUF, (const char*)&flag, sizeof(int));
}//SIZE
int candy_socket_set_sendbuf_size(candy_socket_t sock,int flag){
	return setsockopt( sock, SOL_SOCKET, SO_SNDBUF, (const char*)&flag, sizeof(int));
}//SIZE
