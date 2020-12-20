/* qcl.c by Kevin Dodd */
#include "jesenvsort.h"
#include <curses.h>
#include <errno.h>
#include <fcntl.h>
#include <stdalign.h>
#include <stdlib.h>
#include <term.h>
#include <unistd.h>

static inline int toFailureCode(int e) {
	return (e & 255) ? e : (e | 248);
}

alignas(4096) static char buf[4096];
static size_t bn;
static int f;

static void wr(size_t n) {
	const char* a = buf;
	do {
		ssize_t w = write(f, a, n);
		if (w <= 0) {
			break;
		}
		a += w;
		n -= w;
	} while (n);
}
static int pc(int c) {
	size_t n = bn;
	if (JES_EXPECT_PROB(n >= sizeof buf, 0, 0x0.fffp0)) {
		wr(n);
		n = 0;
	}
	buf[n] = c;
	bn = n+1;
	return 0;
}

int main(int argc, char** argv) {
	(void)argc; /* suppress "unused" warning */
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
		x.a = q;
		x.n = 0;
		while (1) {
			x.a[x.n] = x.a[x.n+1];
			if (!x.a[x.n]) {
				break;
			}
			x.n++;
		}
	}
	q = jesEnvGet("TERM=", x.a, x.n);
	if (isatty(1)) {
		f = 1;
	} else if (isatty(2)) {
		f = 2;
	} else {
		goto SkipClear;
	}
	int r;
	if (setupterm(q ? *q + 5 : NULL, f, &r) == ERR) {
		goto SkipClear;
	}
	r = lines;
	if (r <= 0) { r = 1; }
	if (tputs(clear_screen, r, pc) != ERR) {
		const char* e3 = tigetstr("E3");
		if (e3 && e3+1) {
			(void)tputs(e3, r, pc);
		}
	}
	wr(bn);
SkipClear:
	errno = 0;
	if (!*++argv) {
		return 0;
	}
	close(0);
	if (
		!open("/dev/null", O_RDWR)
		&& dup2(0, 1) >= 0
		&& dup2(0, 2) >= 0
	) {
		execvp(*argv, argv);
	}
	return toFailureCode(errno);
}
