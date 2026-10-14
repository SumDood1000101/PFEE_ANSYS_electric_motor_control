/*
    Target configuration for the Scade One (Swan) generated code.

    Not generated: swan_types.h includes this file and expects it to
    provide the swan_* scalar types and literal macros.
*/

#ifndef SWAN_CONFIG_H_
#define SWAN_CONFIG_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef bool        swan_bool;
typedef int8_t      swan_int8;
typedef int16_t     swan_int16;
typedef int32_t     swan_int32;
typedef int64_t     swan_int64;
typedef uint8_t     swan_uint8;
typedef uint16_t    swan_uint16;
typedef uint32_t    swan_uint32;
typedef uint64_t    swan_uint64;
typedef float       swan_float32;
typedef double      swan_float64;
typedef char        swan_char;
typedef size_t      swan_size;

#define swan_lit_bool(x)        ((swan_bool)(x))
#define swan_lit_int8(x)        ((swan_int8)(x))
#define swan_lit_int16(x)       ((swan_int16)(x))
#define swan_lit_int32(x)       ((swan_int32)(x))
#define swan_lit_int64(x)       ((swan_int64)(x))
#define swan_lit_uint8(x)       ((swan_uint8)(x))
#define swan_lit_uint16(x)      ((swan_uint16)(x))
#define swan_lit_uint32(x)      ((swan_uint32)(x))
#define swan_lit_uint64(x)      ((swan_uint64)(x))
#define swan_lit_float32(x)     ((swan_float32)(x))
#define swan_lit_float64(x)     ((swan_float64)(x))

#endif /* SWAN_CONFIG_H_ */
