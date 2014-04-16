#include "candy/candy_sleep.h"

#if defined CANDY_HAVE_WINDOWS
#include "candy_window.h"
void candy_sleep(int ms){
	Sleep(ms);
}
#else
#include <sys/select.h>

void candy_sleep(int ms){
	struct timeval tv;
	tv.tv_sec = ms/1000;
	tv.tv_usec = ( ms- tv.tv_sec*1000)*1000;
	select(0,0,0,0,&tv);
}
#endif
