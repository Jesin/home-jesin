/* qce.c by Kevin Dodd */
#include "jesenvsort.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static int toFailureCode(int e) {
	return (e & 255) ? e : (e | 248);
}

int main(int argc, char** argv) {
	(void)argc; /* suppress "unused" warning */
	if (!*++argv) { return toFailureCode(EINVAL); }
	close(0);
	if (
		open("/dev/null", O_RDWR)
		|| dup2(0, 1) < 0
		|| dup2(0, 2) < 0
	) { return toFailureCode(errno); }
	JesEnvRangeT x;
	x.a = environ;
	x.n = 0;
	while (x.a[x.n]) { x.n++; }
	if (x.n > 1) {
		char** y = malloc((x.n+1) * sizeof y);
		char*** z = malloc((x.n/2+2) * sizeof y);
		x = jesEnvSort(x.a, y, z);
		free(z);
		if (x.a == y) {
			environ = x.a;
		} else {
			free(y);
		}
	}
	char** q = jesEnvGet("_=", x.a, x.n);
	if (q) {
		x.n = q - x.a;
		do {
			*q = q[1];
		} while (*q++);
	}
	q = jesEnvGet("OLDPWD=", x.a, x.n);
	if (q) {
		do {
			*q = q[1];
		} while (*q++);
	}
	execvp(*argv, argv);
	return toFailureCode(errno);
}
