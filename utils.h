#pragma once
#include <stdint.h>
#include <stdio.h>

#define JS_DEBUG

#ifdef JS_DEBUG
#	define dbgprint(format,args...) \
	fprintf(stderr, format, ##args)
#else
#	define dbgprint(format,args...)
#endif

// TODO:use clean up function instead of exit
#define Js_FatalError(format,args...) \
	fprintf(stderr, format, ##args),\
	exit(-1)

#define CHAR_BIT 8

typedef uint64_t u64;
typedef uint32_t u32;