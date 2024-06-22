#ifndef RGVENGINE_H
#define RGVENGINE_H

#include <cglm/mat4.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float_t f32;
typedef double_t f64;

typedef size_t usize;
typedef ssize_t isize;

typedef void (*rgv_model_manipulations)(mat4);

#define PI 3.14159265359f

#define ASSERT(_e, err ...) if(!(_e)){ printf(err); exit(1);}

#endif
