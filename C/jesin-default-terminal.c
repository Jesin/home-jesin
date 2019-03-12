/* jesin-default-terminal.c by Kevin Dodd */
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 200809L
#endif
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
extern char **environ;

static inline int toFailureCode(int e) {
	return (e & 255) ? e : (e | 248);
}

static const uint64_t *const U64_7BMASK = (const uint64_t *)"\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
static const uint64_t *const OLDPWD_U64 = (const uint64_t *)"OLDPWD=";
static const uint16_t *const USCORE_U16 = (const uint16_t *)"_=";
static char *const EXECPATH = "/usr/bin/termite";
static char *const ARG1 = "-e";
static char *const ARG2 = "clenv tmux new -As0 -nlog journalctl -oshort-precise -fn8192";
static char *const ARGV[] = {EXECPATH + 9, ARG1, ARG2, NULL};

static int spc(const void *a, const void *b) {
	return strcmp(*(const char *const *)a, *(const char *const *)b);
}

int main(void) {
	pid_t x;
	if (
		close(0) < 0
		|| open("/dev/null", O_RDWR) != 0
		|| dup2(0, 1) < 0
		|| dup2(0, 2) < 0
		|| (x = fork()) < 0
	) {
		return toFailureCode(errno);
	}
	if (x > 0) { return 0; }
	if (setsid() < 0) { setpgid(0, 0); }
	if (chdir("/home/jesin") >= 0) {
		putenv("PWD=/home/jesin");
	}
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
	execv(EXECPATH, ARGV);
	return toFailureCode(errno);
}
