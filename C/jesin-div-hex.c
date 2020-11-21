/* jesin-div-hex.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <stdalign.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>

#define JESSTRINGIFY2(x) #x
#define JESSTRINGIFY(x) JESSTRINGIFY2(x)
#define GPREC JESSTRINGIFY(DBL_DECIMAL_DIG)

alignas(4096) static char buf[32768];

static inline unsigned long dbl_as_ul(double x) {
	union { double f; unsigned long i; } t;
	t.f = x;
	return t.i;
}

static inline int toFailureCode(int e) {
	return (e & 255) ? e : (e | 248);
}

static const char calcnprintfmtstr[] =
	"z = %s\n"
	"z: %." GPREC "g\n"
	"z: %a\n"
	"zi 0x%.16lx\n"
;

#define CALCNPRINT(EXPR) do {\
	z = (EXPR);\
	printf(calcnprintfmtstr, #EXPR, z, z, dbl_as_ul(z));\
} while (0)

int main(int argc, const char* const* argv) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s number number\n", argv[0]);
		return toFailureCode(EINVAL);
	}
	__fsetlocking(stdout, FSETLOCKING_BYCALLER);
	setvbuf(stdout, buf, _IOFBF, sizeof buf);
	double x = strtod(argv[1], NULL);
	double y = strtod(argv[2], NULL);
	printf(
		"x = strtod(\"%s\", NULL)\n"
		"x: %." GPREC "g\n"
		"x: %a\n"
		"xi 0x%.16lx\n"
		"y = strtod(\"%s\", NULL)\n"
		"y: %." GPREC "g\n"
		"y: %a\n"
		"yi 0x%.16lx\n"
		, argv[1], x, x, dbl_as_ul(x)
		, argv[2], y, y, dbl_as_ul(y)
	);
	double z;
	CALCNPRINT(x / y);
	CALCNPRINT(remainder(x, y));
	CALCNPRINT(fmod(x, y));
	CALCNPRINT(nextafter(x, y));
	CALCNPRINT(pow(x, y));
	CALCNPRINT(exp(log(x) * y));
	CALCNPRINT(expm1(log(x) * y));
	CALCNPRINT(log1p(y));
	CALCNPRINT(log1p(x) / z);
	CALCNPRINT(atan2(y, x));
	CALCNPRINT(x * x + y * y);
	CALCNPRINT(sqrt(z));
	CALCNPRINT(fma(x, x, y * y));
	CALCNPRINT(sqrt(z));
	CALCNPRINT(hypot(x, y));
	return 0;
}
