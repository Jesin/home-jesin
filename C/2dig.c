/* 2dig.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

static inline int makeFailureCode(int e) {
	return (e & 255) ? e : (e | 248);
}

int main(int argc, char *const *argv) {
	++argv;
	if (--argc < 0) { return makeFailureCode(argc); }
	size_t const n = (size_t)argc;
	char **newargv = malloc((4 * n + 2) * sizeof(char*));
	if (!newargv) { return makeFailureCode(errno); }
	newargv[0] = "dig";
	for (size_t i = 0; i < n; ++i) {
		newargv[4*i + 1] = argv[i];
		newargv[4*i + 2] = "AAAA";
		newargv[4*i + 3] = argv[i];
		newargv[4*i + 4] = "A";
	}
	newargv[4*n + 1] = NULL;
	execvp(newargv[0], newargv);
	int e = errno;
	free(newargv);
	return makeFailureCode(e);
}
