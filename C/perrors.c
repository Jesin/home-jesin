/* perrors.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <stdalign.h>
#include <stdio.h>
#include <string.h>

#define JESBUFSIZE BUFSIZ
#define JESBUFALIGN (4096)

alignas(JESBUFALIGN) char buf[JESBUFSIZE];

int main(void) {
	setvbuf(stdout, buf, _IOFBF, JESBUFSIZE);
	for (int i = 0; i < 135; ++i) {
		char const *s = strerror(i);
		if (!s) { s = "(strerror returned NULL)"; }
		printf("%3d %s\n", i, s);
	}
	return 0;
}
