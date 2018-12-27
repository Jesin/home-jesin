/* nargs.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <errno.h>
#include <stdalign.h>
#include <stdio.h>
#include <unistd.h>

#define JESBUFSIZE (4096)
#define JESBUFALIGN (4096)

alignas(JESBUFALIGN) char buf[JESBUFSIZE];

static inline int toFailureCode(int e) {
	return (e & 255) ? e : (e | 248);
}

int main(int argc, char const *const *const argv) {
	(void)argv; /* suppress "unused" warning */
	--argc;
	ssize_t n = snprintf(buf, JESBUFSIZE, "%d\n", argc);
	if (n <= 0 || n >= JESBUFSIZE) {
		return toFailureCode(errno);
	}
	char const *s = buf;
	do {
		ssize_t k = write(1, s, n);
		if (k <= 0) {
			return toFailureCode(errno);
		}
		s += k;
		n -= k;
	} while (n > 0);
	return 0;
}
