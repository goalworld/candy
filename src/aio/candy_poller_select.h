
#include "../base/candy_window.h"
#include "../base/candy_array.h"
struct candy_poller{
	fd_set rd_set;
	fd_set wr_set;
	fd_set err_set;
	struct candy_array arr;
	int cur;
};
