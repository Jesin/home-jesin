/* jesenvsort.h by Kevin Dodd */
#ifndef jesenvsort_h_INCLUDED
#define jesenvsort_h_INCLUDED 1
#if _POSIX_C_SOURCE < 200809L
#define _POSIX_C_SOURCE 200809L
#endif
#include "jesmacros.h"
extern char** environ;

typedef struct JesEnvRangeT {
	char** a;
	size_t n;
} JesEnvRangeT;

extern __attribute__((nonnull)) int jesEnvCmp(const void*, const void*);
extern __attribute__((nonnull)) char** jesEnvGet(const char* restrict, char** restrict, size_t);
extern __attribute__((nonnull)) JesEnvRangeT jesEnvSort(char** restrict, char** restrict, char*** restrict);

#endif
