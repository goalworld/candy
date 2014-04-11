#include "candy_thread.h"

#if defined CANDY_HAVE_WINDOWS
#include "candy_thread_win.inc"
#else
#include "candy_thread_posix.inc"
#endif
