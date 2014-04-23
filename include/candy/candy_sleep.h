#ifndef CANDY_SLEEP_H_
#define CANDY_SLEEP_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "./candy_export.h"

void candy_sleep(int ms);

long int candy_time_now();

#ifdef __cplusplus
}
#endif
#endif
