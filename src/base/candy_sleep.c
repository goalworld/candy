#include "candy/candy_sleep.h"

#if defined CANDY_HAVE_WINDOWS
#include "candy_window.h"
#include <stdlib.h>
void candy_sleep(int ms){
	Sleep(ms);
}
int 
candy_time_now(){
	return clock();
}
#else
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
void candy_sleep(int ms){
	struct timeval tv;
	tv.tv_sec = ms/1000;
	tv.tv_usec = ( ms- tv.tv_sec*1000)*1000;
	select(0,0,0,0,&tv);
}

int
candy_time_now()
{
	struct timeval time;
	gettimeofday(&time,NULL);
	printf("%ld",time.tv_sec*1000 + time.tv_usec/1000)
	return time.tv_sec*1000 + time.tv_usec/1000;
}
#endif
