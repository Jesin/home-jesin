/* qbg.c by Kevin Dodd */
#include "jesenvsort.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static inline int toFailureCode(int e) {
	return (e & 255) ? e : (e | 248);
}

int main(int argc, char** argv) {
	(void)argc; /* suppress "unused" warning */
	if (!*++argv) { return toFailureCode(EINVAL); }
	if (
		close(0) < 0
		|| open("/dev/null", O_RDWR)
		|| dup2(0, 1) < 0
		|| dup2(0, 2) < 0
	) { return toFailureCode(errno); }
	pid_t i = fork();
	if (i < 0) { return toFailureCode(errno); }
	if (i) { return 0; }
	setsid();
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
	execvp(*argv, argv);
	return toFailureCode(errno);
}
