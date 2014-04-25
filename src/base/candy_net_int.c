#include "candy/candy_net_int.h"

#if defined CANDY_HAVE_WINDOWS
	#include "candy_window.h"
#else
	#include <netinet/in.h>
	#include <arpa/inet.h>
#endif

unsigned short candy_htons(unsigned short val){
	return htons(val);
}
unsigned short candy_ntohs(unsigned short val){
	return ntohs(val);
}
unsigned  candy_htonl(unsigned  val){
	return htonl(val);
}
unsigned  candy_ntohl(unsigned  val){
	return ntohl(val);
}
