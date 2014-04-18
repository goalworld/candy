
#ifndef _CANDY_ARRAY_H_
#define _CANDY_ARRAY_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "./candy_export.h"

struct candy_array
{
	void *arr_data;
	int elem_sz;
	int arr_sz;
	int cut_sz;
};
CANDY_EXPORT void candy_array_init(struct candy_array *arr,int elemsz);
CANDY_EXPORT void candy_array_destroy(struct candy_array *arr);
CANDY_EXPORT void candy_array_push(struct candy_array *arr,void *elem);
CANDY_EXPORT void candy_array_at(struct candy_array *arr,int index,void *elem);
CANDY_EXPORT void candy_array_set(struct candy_array *arr,int index,void *elem);
CANDY_EXPORT void candy_array_unshift(struct candy_array *arr,void *elem);
CANDY_EXPORT void candy_array_earse(struct candy_array *arr,int index,int numelem,void *elem);
CANDY_EXPORT void candy_array_insert(struct candy_array *arr,int index,void *elem,int numelem);
CANDY_EXPORT unsigned candy_array_size(struct candy_array *arr);

#ifdef __cplusplus
}
#endif
#endif 
