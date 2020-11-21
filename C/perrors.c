/* perrors.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <stdalign.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>

alignas(4096) static char buf[8192];

int main(void) {
	__fsetlocking(stdout, FSETLOCKING_BYCALLER);
	setvbuf(stdout, buf, _IOFBF, sizeof buf);
	for (int i = 0; i < 135; ++i) {
		char const *s = strerror(i);
		if (!s) { s = "(strerror returned NULL)"; }
		printf("%3d %s\n", i, s);
	}
	return 0;
}
