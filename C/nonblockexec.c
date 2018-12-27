/* nonblockexec.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static inline int toFailureCode(int e) {
	return (e & 255) ? e : (e | 248);
}

static inline unsigned max_uint(unsigned a, unsigned b) {
	return a < b ? b : a;
}

int main(int argc, char *const *argv) {
	++argv;
	if (--argc < 0) { return toFailureCode(argc); }
	int *fds = malloc(sizeof(int) * (size_t)max_uint(argc, 3));
	if (!fds) { return toFailureCode(ENOMEM); }
	unsigned n = 0;
	while (argv[n] && sscanf(argv[n], "%i", &fds[n]) > 0) { ++n; }
	if (argv[n] && !argv[n][0]) {
		argv = &argv[n+1];
	} else {
		for (n = 0; n < 3; ++n) { fds[n] = n; }
	}
	for (unsigned i = 0; i < n; ++i) {
		int flags = fcntl(fds[i], F_GETFL);
		if (flags < 0 || (flags & O_NONBLOCK)) { continue; }
		flags |= O_NONBLOCK;
		fcntl(fds[i], F_SETFL, flags);
	}
	free(fds); fds = NULL;
	execvp(argv[0], argv);
	return toFailureCode(errno);
}
