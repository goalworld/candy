/*
 * rain_array.c
 *
 *  Created on: 2012-11-14
 *      Author: goalworld
 */


#include "candy_array.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
void
candy_array_init(struct candy_array *arr,int elemsz)
{
	assert(arr);
	arr->elem_sz = elemsz;
	arr->arr_data = malloc(elemsz * 64);
	arr->arr_sz = 64;
	arr->cut_sz = 0;
}
static inline void
_add_size(struct candy_array *arr,int elem)
{
	if(elem <= arr->arr_sz){
		return;
	}
	int sz = arr->arr_sz*2;
	for(;;){
		if(sz >= elem ){
			arr->arr_data = realloc(arr->arr_data,arr->elem_sz * sz);
			arr->arr_sz = sz;
			break;
		}
		sz*=2;
	}
}
void
candy_array_destroy(struct candy_array *arr)
{
	assert(arr);
	free(arr->arr_data);
}
void
candy_array_push(struct candy_array *arr,void *elem)
{
	assert(arr);
	_add_size(arr,arr->cut_sz+1);
	memcpy((char*)(arr->arr_data)+arr->cut_sz*arr->elem_sz,
			elem,
			arr->elem_sz);
	arr->cut_sz++;
}
void
candy_array_at(struct candy_array *arr,int index,void *elem)
{
	assert(arr && index<arr->cut_sz);
	memcpy(elem,
			(char*)(arr->arr_data)+arr->elem_sz*index,
			arr->elem_sz);
}
void
candy_array_set(struct candy_array *arr,int index,void *elem)
{
	assert(arr && index<=arr->cut_sz);
	memcpy((char*)(arr->arr_data)+arr->elem_sz*index,
			elem,
			arr->elem_sz);
}
void
candy_array_unshift(struct candy_array *arr,void *elem)
{
	assert(arr);
	_add_size(arr,arr->cut_sz+1);
	memmove((char*)(arr->arr_data)+1*arr->elem_sz,
			(char*)(arr->arr_data),
			arr->cut_sz*arr->elem_sz);
	memcpy((char*)(arr->arr_data),
			elem,
			arr->elem_sz);
	arr->cut_sz++;
}
void
candy_array_earse(struct candy_array *arr,int index,int numelem,void *elem)
{
	assert(arr && index<=arr->cut_sz && numelem <= arr->cut_sz-index);
	if(elem){
		memcpy(elem,
				(char*)(arr->arr_data) + (index)*arr->elem_sz,
				numelem*arr->elem_sz);
	}
	if((arr->cut_sz-index-numelem)*arr->elem_sz){
		memmove((char*)(arr->arr_data) + index*arr->elem_sz,
			(char*)(arr->arr_data) + (index+numelem)*arr->elem_sz,
			(arr->cut_sz-index-numelem)*arr->elem_sz);
	}
	arr->cut_sz-=numelem;
}
void
candy_array_insert(struct candy_array *arr,int index,void *elem,int numelem)
{
	assert(arr && index<=arr->cut_sz);
	_add_size(arr,arr->cut_sz+numelem);
	memmove((char*)(arr->arr_data) + (index+numelem)*arr->elem_sz,
			(char*)(arr->arr_data) + (index)*arr->elem_sz,
			(arr->cut_sz-index)*arr->elem_sz);
	memcpy((char*)(arr->arr_data) + (index)*arr->elem_sz,
			elem,
			numelem*arr->elem_sz);
	arr->cut_sz+=numelem;
}
unsigned
candy_array_size(struct candy_array *arr)
{
	assert(arr);
	return arr->cut_sz;
}
