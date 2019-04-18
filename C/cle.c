/* cle.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
extern char **environ;

static inline int toFailureCode(int e) {
	return (e & 255) ? e : (e | 248);
}

static const uint64_t *const U64_7BMASK = (const uint64_t *)"\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
static const uint64_t *const OLDPWD_U64 = (const uint64_t *)"OLDPWD=";
static const uint16_t *const USCORE_U16 = (const uint16_t *)"_=";

static int spc(const void *a, const void *b) {
	return strcmp(*(const char *const *)a, *(const char *const *)b);
}

int main(int argc, char *const argv[]) {
	(void)argc; /* suppress "unused" warning */
	if (!(argv && argv[0] && argv[1])) { return -1; }
	size_t i = 0;
	for (size_t k = 0; environ[i];) {
		++k;
		if (*(uint16_t*)environ[i] != *USCORE_U16 && (*(uint64_t*)environ[i] & *U64_7BMASK) != *OLDPWD_U64) {
			++i;
		}
		if (i != k) {
			environ[i] = environ[k];
		}
	}
	qsort(environ, i, sizeof(char*), spc);
	execvp(argv[1], &argv[1]);
	return toFailureCode(errno);
}
