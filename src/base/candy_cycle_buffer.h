#ifndef CANDY_CYCLE_BUFFER_H_
#define CANDY_CYCLE_BUFFER_H_ 
#include "./candy_export.h"
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
#define candy_cycle_buffer_empty(self) (self)->has_data_

#endif

