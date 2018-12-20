/* jessetsid.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static inline int toFailureCode(int e) {
	return (e & 255) ? e : (e | 248);
}

int main(int argc, char *const argv[]) {
	(void)argc; /* suppress "unused" warning */
	if (!(argv && argv[0] && argv[1])) { return -1; }
	if (setsid() < 0) {
		pid_t x = fork();
		if (x > 0) { return 0; }
		if (x < 0) { return toFailureCode(errno); }
		setsid();
	}
	execvp(argv[1], &argv[1]);
	int e = errno;
	fprintf(stderr, "%s failed to exec %s: %s\n", argv[0], argv[1], strerror(e));
	return toFailureCode(e);
}
