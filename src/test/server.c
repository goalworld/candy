#include "../candy.h"
#include "../base/candy_log.h"
#include "../base/candy_sleep.h"

static int num_recv;
static int num_socket;
void connect_fn(void* arg){
	int sock = (int)arg;
	CANDY_INFO("connect_fn%i",sock);
	char buf[] = "hello";
	int rc = candy_send(sock,buf,sizeof(buf));
	CANDY_INFO("candy_send rc:%d",rc);
}
void message_fn(void* arg,void* data,int len){

	int sock = (int)arg;
	//CANDY_INFO("message_fn%i",sock);
	num_recv++;
	char buf[] = "world";
	candy_send(sock,buf,sizeof(buf));
}
void drain_fn(void* arg){
	int sock = (int)arg;
	CANDY_INFO("drain_fn%i",sock);
}
void close_fn(void* arg,int code){
	int sock = (int)arg;
	CANDY_INFO("close_fn sock:%icode:%d",sock,code);
	candy_close(sock);
	num_socket--;
}
void accept_fn(void* arg,int s){
	num_socket++;
//	CANDY_INFO("accept_fn%i %d",sock,s);
	struct candy_callback cb;
	cb.connect_fn = connect_fn;
	cb.message_fn = message_fn;
	cb.drain_fn = drain_fn;
	cb.close_fn = close_fn;
	cb.arg = (void*)s;
	candy_set_callback(s,cb);
}

int main(){
	candy_start(4);
	int io = candy_socket();
	if(io < 0){
		CANDY_INFO("candy_socket error%i",io);
		return -1;
	}
	struct candy_callback cb;
	cb.close_fn = close_fn;
	cb.accept_fn = accept_fn;
	cb.arg = (void*)io;
	int rc;
	if( (rc = candy_listen(io,"0.0.0.0",1922) ) != 0){
		CANDY_INFO("candy_listen error%i",rc);
		return -1;
	}
	CANDY_INFO("candy_listen ok port:%d",1922);
	candy_set_callback(io,cb);
	while(1){
		if(num_recv > 0){
			CANDY_DEBUG("num_recv:%d,num_socket:%d",num_recv,num_socket);
			num_recv=0;
		}
		candy_sleep(1000);
	}
	return 0;
}
