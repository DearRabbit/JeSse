#pragma once

#include <stdio.h>

#ifdef JS_DEBUG
#define dbgprint(format,args...) \
	fprintf(stderr, format, ##args)
#else
	#define dbgprint(format,args...)
#endif

