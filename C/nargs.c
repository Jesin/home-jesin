/* nargs.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <errno.h>
#include <stdalign.h>
#include <stdio.h>

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
	setvbuf(stdout, buf, _IOFBF, sizeof(buf));
	if (printf("%u\n", (unsigned)argc) <= 0) {
		return toFailureCode(errno);
	}
	return 0;
}
