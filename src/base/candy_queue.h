
#ifndef _candy__QUEUE_H_
#define _candy__QUEUE_H_

struct candy_queue
{
	void * q_buf;
	int q_cut;
	int q_end;
	int q_len;
	int q_elemsize;
};
typedef void candy_queue_elem_del_fn(void *elem);
int 		candy_queue_init		(struct candy_queue *que, unsigned elemsize);
void 		candy_queue_destroy	(struct candy_queue *que, candy_queue_elem_del_fn fn);
void 		candy_queue_push		(struct candy_queue *que, void *elem);
int 		candy_queue_pop		(struct candy_queue *que, void *elem);
unsigned 	candy_queue_size		(struct candy_queue *que );
#endif /* RAIN_QUEUE_H_ */
