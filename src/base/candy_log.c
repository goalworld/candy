/*
 * rain_loger.c
 *
 *  Created on: 2012-11-17
 *      Author: goalworld
 */

#include "candy_log.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
//static FILE * file = NULL;
int g_candy_log_level = 3;
void
candy_log(const char *pre,const char *filename,int line,const char *fmt,...)
{
	char buf[8096];
	va_list args;
	va_start(args,fmt);
	vsnprintf(buf,sizeof(buf),fmt,args);
	va_end(args);
	char date_str[64];
	time_t t;
	struct tm *p;
	t = time(NULL);
	p = localtime(&t);
	//if(!file){
	//	file = fopen(".\\loger.txt","w");
	//}
	sprintf(date_str, "%d-%02d-%02d %02d:%02d:%02d", p->tm_year + 1900, p->tm_mon+1, p->tm_mday , p->tm_hour, p->tm_min, p->tm_sec);
	fprintf(stdout, "[CANDY %s %s %s:%d] %s\r\n",date_str,pre, filename, line, buf);
	fflush(stdout);  // Needed on MSVC.
	//if(file){
	//	fprintf(file, "[CANDY %s %s %s:%d] %s\r\n",date_str,pre, filename, line, buf);
	//	fflush(file);  // Needed on MSVC.
	//}
}
