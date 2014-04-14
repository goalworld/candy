/*
 * rain_loger.h
 *
 *  Created on: 2012-11-17
 *      Author: goalworld
 */

#ifndef CANDY_LOGER_H_
#define CANDY_LOGER_H_

#define CANDY_LOG_ERROR 0
#define CANDY_LOG_WARN 1
#define CANDY_LOG_DEBUG 2
#define CANDY_LOG_INFO -1

extern int g_candy_log_level;

#define CANDY_LOG(pre,lvl,fmt,...)\
		do{\
			if(lvl <= g_candy_log_level){\
				candy_log(pre,__FILE__,__LINE__,fmt,__VA_ARGS__);	\
			}						      \
		}while(0)

#define CANDY_DEBUG(fmt,...) CANDY_LOG("DEBUG",CANDY_LOG_DEBUG,fmt,__VA_ARGS__)
#define CANDY_INFO(fmt,...) CANDY_LOG("INFO",CANDY_LOG_INFO,fmt,__VA_ARGS__)
#define CANDY_ERROR(fmt,...) CANDY_LOG("ERROR",CANDY_LOG_ERROR,fmt,__VA_ARGS__)
#define CANDY_WARN(fmt,...) CANDY_LOG("WARN",CANDY_LOG_WARN,fmt,__VA_ARGS__)

void candy_log(const char *pre,const char *filename,int line,const char *fmt,...);

#define CANDY_CHECK(ep) if(!(ep)){CANDY_ERROR("CANDY_CHECK_FAIL:%s",#ep);}

#endif /* CANDY_LOGER_H_ */
