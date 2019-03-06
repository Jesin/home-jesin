/* quietsetsid.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
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
	int errfd = fcntl(2, F_DUPFD_CLOEXEC, 3);
	if (
		errfd < 0
		|| close(0) < 0
		|| open("/dev/null", O_RDWR) != 0
		|| dup2(0, 1) < 0
		|| dup2(0, 2) < 0
	) { return toFailureCode(errno); }
	if (setsid() < 0) {
		pid_t x = fork();
		if (x > 0) { return 0; }
		if (x < 0) { return toFailureCode(errno); }
		setsid();
	}
	size_t i = 0;
	while (environ[i]) { ++i; }
	qsort(environ, i, sizeof(char*), spc);
	execvp(argv[1], &argv[1]);
	int e = errno;
	dprintf(errfd, "%s failed to exec %s: %s\n", argv[0], argv[1], strerror(e));
	return toFailureCode(e);
}
