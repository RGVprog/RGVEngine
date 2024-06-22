// AUTHOR: RGVprog

//  THIS IS A SIMPLE DYNAMIC ARRAY IMPLEMENTATION IN C99
//  JO MAMA CAN SUGGONDEEZNUTS

#ifndef D_ARRAY_H
#define D_ARRAY_H

#include <sys/types.h>
#include <stdlib.h>

typedef struct { 
    void *ptr; 
    size_t capacity;
    size_t size;
    size_t len;
}d_array;

d_array d_array_create (size_t sizeoftype, size_t len);
void d_array_free(d_array*);

#define d_array_put(type, index, ptr, value) (((type*)ptr)[index] = value)

#define d_array_at(type, index, ptr) (((type*)ptr)[index])

#define d_array_push(d_arr, type, value) {\
    d_arr.size += sizeof(type);\
    while(d_arr.size > d_arr.capacity){d_arr.capacity *= 2;}\
    d_arr.len = d_arr.size/sizeof(type);\
    d_arr.ptr = realloc(d_arr.ptr, d_arr.capacity);\
    d_array_put(type, (d_arr.size/sizeof(type)) - 1, d_arr.ptr, value);}
/*
#define d_array_insert(d_arr, type, d_arr2) {\
    for (int i = 0; i < d_arr2.len; i++)\
    d_array_push(d_arr, type, d_array_at(type, i, d_arr2.ptr));}
*/

#endif
