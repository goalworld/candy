#ifndef CANDY_WIN_INCLUDED
#define CANDY_WIN_INCLUDED
#if defined CANDY_HAVE_WINDOWS
	
	#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#endif

	#include <windows.h>
	#include <winsock2.h>
	#include <mswsock.h>
	#include <process.h>
	#include <ws2tcpip.h>

#endif
#endif

