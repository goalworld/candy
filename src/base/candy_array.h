
#ifndef _CANDY_ARRAY_H_
#define _CANDY_ARRAY_H_


struct candy_array
{
	void *arr_data;
	int elem_sz;
	int arr_sz;
	int cut_sz;
};
void candy_array_init(struct candy_array *arr,int elemsz);
void candy_array_destroy(struct candy_array *arr);
void candy_array_push(struct candy_array *arr,void *elem);
void candy_array_at(struct candy_array *arr,int index,void *elem);
void candy_array_set(struct candy_array *arr,int index,void *elem);
void candy_array_unshift(struct candy_array *arr,void *elem);
void candy_array_earse(struct candy_array *arr,int index,int numelem,void *elem);
void candy_array_insert(struct candy_array *arr,int index,void *elem,int numelem);
unsigned candy_array_size(struct candy_array *arr);

#endif 
