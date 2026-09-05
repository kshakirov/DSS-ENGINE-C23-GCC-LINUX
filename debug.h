#pragma once
#include <stdio.h>
#ifdef  DSS_DEBUG
#define DEBUG_LOG(...) fprintf(stderr, __VA_ARGS__)
#else
#define DEBUG_LOG(...) ((void)0)
#endif
