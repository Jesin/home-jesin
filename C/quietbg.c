/* quietbg.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static inline int makeFailureCode(int e) {
	return (e & 255) ? e : (e | 248);
}

int main(int argc, char *const argv[]) {
	(void)argc; /* suppress "unused" warning */
	if (!(argv && argv[0] && argv[1])) { return -1; }
	int errfd = fcntl(2, F_DUPFD_CLOEXEC, 3);
	if (errfd < 0) { return makeFailureCode(errno); }
	int devnull = open("/dev/null", O_RDWR | O_CLOEXEC);
	if (devnull < 0) { return makeFailureCode(errno); }
	if (devnull <= 2) {
		int r = fcntl(devnull, F_GETFD);
		if (r < 0) { return makeFailureCode(errno); }
		r = fcntl(devnull, F_SETFD, r & ~FD_CLOEXEC);
		if (r < 0) { return makeFailureCode(errno); }
	}
	for (int fd = 0; fd <= 2; ++fd) {
		if (dup2(devnull, fd) < 0) { return makeFailureCode(errno); }
	}
	pid_t x = fork();
	if (x > 0) { return 0; }
	if (x < 0) { return makeFailureCode(errno); }
	setsid();
	execvp(argv[1], &argv[1]);
	int e = errno;
	dprintf(errfd, "%s failed to exec %s: %s\n", argv[0], argv[1], strerror(e));
	return makeFailureCode(e);
}
