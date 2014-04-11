#include <sys/epoll.h>

#define CANDY_EPOLL_REVENT_SIZE 64
struct candy_poller{
	int efd;
	struct epoll_event revent[CANDY_EPOLL_REVENT_SIZE];
};
