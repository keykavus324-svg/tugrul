#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef u32 size_t;
typedef s32 ssize_t;

typedef u32 uintptr_t;
typedef s32 intptr_t;

#define NULL ((void*)0)
#define TRUE 1
#define FALSE 0

typedef int bool_t;

#endif
