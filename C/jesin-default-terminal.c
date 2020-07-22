/* jesin-default-terminal.c by Kevin Dodd */
#include "jesenvsort.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static inline int toFailureCode(int e) {
	return (e & 255) ? e : (e | 248);
}

static char* const EXE = "/usr/bin/termite";
static char* const HOME = "HOME=/tmp";

int main(int argc, char** argv) {
	if (
		close(0) < 0
		|| open("/dev/null", O_RDWR) != 0
		|| dup2(0, 1) < 0
		|| dup2(0, 2) < 0
	) { return toFailureCode(errno); }
	if (setsid() < 0) {
		pid_t i = fork();
		if (i < 0) { return toFailureCode(errno); }
		if (i) { return 0; }
		setsid();
	}
	JesEnvRangeT x;
	x.a = environ;
	x.n = 0;
	while (x.a[x.n]) { x.n++; }
	x.a = malloc((x.n+3) * sizeof x.a);
	memcpy(x.a, environ, x.n * sizeof x.a);
	x.a[x.n] = HOME;
	x.a[x.n+1] = "PWD=";
	x.a[x.n+2] = NULL;
	environ = x.a;
	char** y = malloc((x.n+3) * sizeof y);
	char*** z = malloc((x.n/2+3) * sizeof y);
	x = jesEnvSort(x.a, y, z);
	if (x.a == y) {
		y = environ;
		environ = x.a;
	}
	free(y);
	free(z);
	y = jesEnvGet("_=", x.a, x.n);
	if (y) {
		x.n = y - x.a;
		do {
			*y = y[1];
		} while (*y++);
	}
	y = x.a;
	x.a = jesEnvGet(HOME, x.a, x.n);
	x.n -= x.a - y;
	char* h = *x.a+5;
	y = jesEnvGet("OLDPWD=", x.a, x.n);
	if (y) {
		x.a = y;
		x.n = 0;
		while (1) {
			x.a[x.n] = x.a[x.n+1];
			if (!x.a[x.n]) {
				break;
			}
			x.n++;
		}
	}
	if (chdir(h) >= 0) {
		x.a = jesEnvGet("PWD=", x.a, x.n);
		x.n = strlen(h);
		if (strlen(*x.a+4) < x.n) {
			*x.a = memcpy(malloc(x.n+5), "PWD=", 4);
		}
		memcpy(*x.a+4, h, x.n+1);
	}
	x.a = malloc((3+(size_t)argc) * sizeof y);
	*x.a = EXE + 9;
	x.a[1] = "-e";
	x.a[2] = "cle tmux new -As0 -nlog journalctl -oshort-precise -fn8192";
	memcpy(x.a+3, argv+1, (size_t)argc * sizeof y);
	execv(EXE, x.a);
	free(x.a);
	return toFailureCode(errno);
}
