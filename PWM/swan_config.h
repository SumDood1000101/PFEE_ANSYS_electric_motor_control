// swan_config.h
#ifndef SWAN_CONFIG_H_
#define SWAN_CONFIG_H_

#include <stdint.h>
#include <stdbool.h>

typedef int32_t  swan_int32;
typedef float    swan_float32;
typedef bool     swan_bool;

#define swan_lit_int32(x)  ((swan_int32)(x))
#define swan_lit_float32(x) ((swan_float32)(x))

#ifdef __cplusplus
  #define restrict __restrict__
#endif

#endif