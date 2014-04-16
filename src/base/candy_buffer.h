#ifndef CANDY_BUFFER_H_
#define CANDY_BUFFER_H_

typedef int (*candy_buffer_writer_fn)(void* arg,void *buf,int size);

struct candy_buffer{
	char* data;
	int wr;
	int rd;
	int size;	
};
void candy_buffer_init(struct candy_buffer* buf,int size);
void candy_buffer_write(struct candy_buffer* buf,void *data,int size);
int candy_buffer_read(struct candy_buffer* buf,void *data,int size);
int candy_buffer_read_to_writer(struct candy_buffer* buf,candy_buffer_writer_fn fn,void* arg);
#define candy_buffer_data_size(buf) (buf)->wr - (buf)->rd
#define candy_buffer_free_size(buf) (buf)->size - (buf)->wr
#define candy_buffer_rd_size(buf) (buf)->rd
void candy_buffer_destroy(struct candy_buffer* buf);
#endif