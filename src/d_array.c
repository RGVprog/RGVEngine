#include "../include/d_array.h"

d_array d_array_create (size_t sizeoftype, size_t len){
    d_array a = {
        .ptr = malloc(sizeoftype * len),
        .capacity = sizeoftype,
        .size = sizeoftype * len,
        .len = len,
    };

    return a;
}

void d_array_free(d_array* a){
    free(a->ptr);
    a->ptr = NULL;
}
