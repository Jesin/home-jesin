/* clr.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <curses.h>
#include <errno.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <term.h>
#include <unistd.h>
extern char **environ;

static inline int toFailureCode(int e) {
	return (e & 255) ? e : (e | 248);
}

alignas(4096) static char buf[4096];
static FILE *fp;
static int fd;

static const uint64_t *const U64_7BMASK = (const uint64_t *)"\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
static const uint64_t *const OLDPWD_U64 = (const uint64_t *)"OLDPWD=";
static const uint16_t *const USCORE_U16 = (const uint16_t *)"_=";

static int ptc(int c) {
	return putc(c, fp);
}
static int spc(const void *a, const void *b) {
	return strcmp(*(const char *const *)a, *(const char *const *)b);
}

int main(int argc, char *const argv[]) {
	(void)argc; /* suppress "unused" warning */
	size_t i = 0;
	for (size_t k = 0; environ[i];) {
		++k;
		if (*(uint16_t*)environ[i] != *USCORE_U16 && (*(uint64_t*)environ[i] & *U64_7BMASK) != *OLDPWD_U64) {
			++i;
		}
		if (i != k) {
			environ[i] = environ[k];
		}
	}
	qsort(environ, i, sizeof(char*), spc);
	if (isatty(STDOUT_FILENO)) {
		fd = STDOUT_FILENO;
		fp = stdout;
	} else if (isatty(STDERR_FILENO)) {
		fd = STDERR_FILENO;
		fp = stderr;
	} else {
		fd = -1;
	}
	if (fd >= 0) {
		setvbuf(fp, buf, _IOFBF, sizeof(buf));
		int n;
		if (setupterm(NULL, fd, &n) != ERR) {
			n = lines;
			if (n <= 0) { n = 1; }
			if (tputs(clear_screen, n, ptc) != ERR) {
				const char *e3 = tigetstr("E3");
				if (e3) {
					(void)tputs(e3, n, ptc);
				}
			}
			fflush(fp);
		}
	}
	errno = 0;
	if (!(argv && argv[0] && argv[1])) {
		return 0;
	}
	execvp(argv[1], &argv[1]);
	return toFailureCode(errno);
}
