#pragma once

#define JS_DEBUG

#ifdef JS_DEBUG
#define dbgprint(format,args...) \
	fprintf(stderr, format, ##args)
#else
	#define dbgprint(format,args...)
#endif

#define FATAL_ERROR(format,args...) \
	fprintf(stderr, format, ##args),\
	exit(-1)


#define CHAR_BIT 8
