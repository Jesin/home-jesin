/* exec.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
extern char **environ;

static inline int toFailureCode(int e) {
	return (e & 255) ? e : (e | 248);
}

static int spc(const void *a, const void *b) {
	return strcmp(*(const char *const *)a, *(const char *const *)b);
}

int main(int argc, char *const argv[]) {
	(void)argc; /* suppress "unused" warning */
	if (!(argv && argv[0] && argv[1])) { return -1; }
	size_t i = 0;
	while (environ[i]) { ++i; }
	qsort(environ, i, sizeof(char*), spc);
	execvp(argv[1], &argv[1]);
	return toFailureCode(errno);
}
