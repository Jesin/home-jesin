/* nargs.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <errno.h>
#include <stdalign.h>
#include <stdio.h>
#include <unistd.h>

alignas(4096) static char buf[4096];

static inline int toFailureCode(int e) {
	return (e & 255) ? e : (e | 248);
}

int main(int argc, char const *const *const argv) {
	(void)argv; /* suppress "unused" warning */
	if (!argc) {
		return toFailureCode(EINVAL);
	}
	--argc;
	ssize_t n = snprintf(buf, sizeof(buf), "%u\n", (unsigned)argc);
	if (n <= 0 || (size_t)n >= sizeof(buf)) {
		return toFailureCode(errno);
	}
	char const *s = buf;
	do {
		ssize_t k = write(STDOUT_FILENO, s, n);
		if (k <= 0) {
			return toFailureCode(errno);
		}
		s += k;
		n -= k;
	} while (n > 0);
	return 0;
}
