#include "candy/candy.h"
#include "candy/candy_log.h"
#include "candy/candy_sleep.h"
#include <stdlib.h>
#include <memory.h>
#define MAX_SECOND 1000000
struct client{
	int sock;
	int num;
	int index;	
};
static struct client ** clis;
static int cur;
static int start_client();
static void connect_fn(void* arg){
	struct client* cli = (struct client*) arg;
	CANDY_INFO("connect_fn%i",cli->sock);
	char buf[] = "hello";
	int rc = candy_send(cli->sock,buf,sizeof(buf));
	CANDY_INFO("candy_send rc:%d",rc);
}
static void message_fn(void* arg,void* data,int len){
	struct client* cli = (struct client*) arg;
//	CANDY_INFO("message_fn%i:%s",sock,(char*)data);
	if(cli->num++ > MAX_SECOND){
		return;
	}
	char buf[] = "hello";
	candy_send(cli->sock,buf,sizeof(buf));
}
static void drain_fn(void* arg){
	struct client* cli = (struct client*) arg;
	CANDY_INFO("drain_fn%i",cli->sock);
}
static void close_fn(void* arg,int code){
	struct client* cli = (struct client*) arg;
	CANDY_INFO("close_fn%i %d",cli->sock,code);
	candy_close(cli->sock);
	clis[cli->index] = NULL;
	free(cli);
}

int main(int argc,char* argv[]){
	candy_start(4);
	int max = 1;
	int i=0;
	if(argc == 2){
		max = atoi(argv[1]);
	}
	clis = (struct client** ) malloc(sizeof(struct client*)*max);
	memset(clis,0,sizeof(struct client*)*max);
	for(;i<max;i++){
		start_client();
	}
	while(1){
		for(i=0;i<max;i++){
			if(clis[i]){
				clis[i]->num = 0;
				char buf[] = "hello";
				candy_send(clis[i]->sock,buf,sizeof(buf));
			}
				
		}
		candy_sleep(1000);
	}
	candy_wait();
	return 0;
}

int start_client(){
	int io = candy_aio();
	if(io < 0){
		CANDY_INFO("error%i",io);
		return -1;
	}
	struct client* cli =(struct client* ) malloc(sizeof(struct client));
	cli->sock = io;
	cli->num = 0;
	cli->index = cur;
	clis[cur++] = cli;
	struct candy_callback cb;
	cb.connect_fn = connect_fn;
	cb.message_fn = message_fn;
	cb.drain_fn = drain_fn;
	cb.close_fn = close_fn;
	cb.arg =  cli;
	candy_set_callback(io,cb);
	candy_connect(io,"127.0.0.1",1922,10000);
	return 0;
}
