#ifndef CANDY_CYCLE_BUFFER_H_
#define CANDY_CYCLE_BUFFER_H_ 
#include "./candy_export.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef int(*candy_io_fn)(void *arg,void *buf,int size) ;

struct candy_cycle_buffer{
	unsigned char * data_;
	int start_;
	int end_;
	int size_;
	int has_data_;
};

CANDY_EXPORT void candy_cycle_buffer_init(struct candy_cycle_buffer* self);
CANDY_EXPORT void candy_cycle_buffer_destroy(struct candy_cycle_buffer* self);
CANDY_EXPORT int candy_cycle_buffer_read(struct candy_cycle_buffer* self,void *buf,int sz);
CANDY_EXPORT void candy_cycle_buffer_write(struct candy_cycle_buffer* self,void *buf,int sz);
CANDY_EXPORT int candy_cycle_buffer_free_size(struct candy_cycle_buffer* self);
CANDY_EXPORT int candy_cycle_buffer_data_size(struct candy_cycle_buffer* self);
CANDY_EXPORT int candy_cycle_buffer_write_from_reader(struct candy_cycle_buffer* self,candy_io_fn fn,void *arg);
CANDY_EXPORT int candy_cycle_buffer_read_to_writer(struct candy_cycle_buffer* self,candy_io_fn fn,void *arg);
#define candy_cycle_buffer_empty(self) (self)->has_data_

#ifdef __cplusplus
}
#endif
#endif

