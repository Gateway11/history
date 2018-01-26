//
//  typedefs.h
//  vsys
//
//  Created by 代祥 on 2017/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//

#ifndef __vsys_typedefs__
#define __vsys_typedefs__

#include <inttypes.h>
typedef uint8_t vsys_bool_t;        /* unsigned 8 bits */
typedef uint8_t vsys_char_t;        /* unsigned 8 bits */
typedef int16_t vsys_short_t;       /* signed 16 bits */
typedef uint32_t vsys_uint_t;       /* unsigned 32 bits */
typedef int32_t vsys_int_t;         /* signed 32 bits */
typedef int64_t vsys_long_t;        /* signed 64 bits */
typedef float vsys_float_t;         /* 32-bit IEEE 754 */
typedef double vsys_double_t;       /* 64-bit IEEE 754 */
typedef vsys_uint_t vsys_size_t;

#define VSYS_NULL 0x0
#define VSYS_FALSE 0
#define VSYS_TRUE 1

#endif /* __vsys_typedefs__ */
