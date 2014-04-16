#ifndef CANDY_NET_INT_
#define CANDY_NET_INT_

#if defined CANDY_HAVE_WINDOWS
	#include "candy_window.h"
	#define CANDY_HTONS htons
	#define CANDY_NTOHS ntohs
	#define CANDY_HTONL htonl
	#define CANDY_NTOHL ntohl
#else
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#define CANDY_HTONS htons
	#define CANDY_NTOHS ntohs
	#define CANDY_HTONL htonl
	#define CANDY_NTOHL ntohl
#endif