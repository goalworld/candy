#ifndef CANDY_BUFFER_H_
#define CANDY_BUFFER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "./candy_export.h"

typedef int (*candy_buffer_writer_fn)(void* arg,void *buf,int size);

struct candy_buffer{
	char* data;
	int wr;
	int rd;
	int size;	
};
#define candy_buffer_data_size(buf) ((buf)->wr - (buf)->rd)
#define candy_buffer_free_size(buf) ((buf)->size - (buf)->wr)
#define candy_buffer_rd_size(buf) (buf)->rd
#define candy_buffer_data_ptr(buf)((buf)->data + (buf)->rd)
CANDY_EXPORT void candy_buffer_init(struct candy_buffer* buf,int size);
CANDY_EXPORT void candy_buffer_write(struct candy_buffer* buf,void *data,int size);
CANDY_EXPORT int candy_buffer_read(struct candy_buffer* buf,void *data,int size);
CANDY_EXPORT int candy_buffer_read_to_writer(struct candy_buffer* buf,candy_buffer_writer_fn fn,void* arg);
CANDY_EXPORT void candy_buffer_destroy(struct candy_buffer* buf);
CANDY_EXPORT void candy_buffer_clear(struct candy_buffer* buf);

#ifdef __cplusplus
}
#endif
#endif

