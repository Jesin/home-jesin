/* jesin-div-hex.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <errno.h>
#include <float.h>
#include <stdio.h>

#define JESSTRINGIFY2(x) #x
#define JESSTRINGIFY(x) JESSTRINGIFY2(x)

static inline int toFailureCode(int e) {
	return (e & 255) ? e : (e | 248);
}

int main(int argc, char const *const *argv) {
	double x, y;
	if (argc != 3 || sscanf(argv[1], "%lg", &x) != 1 || sscanf(argv[2], "%lg", &y) != 1) {
		fprintf(stderr, "Usage: %s number number\n", argv[0]);
		return toFailureCode(EINVAL);
	}
	double z = x / y;
	printf(
		"x = %." JESSTRINGIFY(DBL_DECIMAL_DIG) "g\n"
		"x = %a\n"
		"y = %." JESSTRINGIFY(DBL_DECIMAL_DIG) "g\n"
		"y = %a\n"
		"z = %." JESSTRINGIFY(DBL_DECIMAL_DIG) "g\n"
		"z = %a\n"
		, x, x, y, y, z, z
	);
	return 0;
}
