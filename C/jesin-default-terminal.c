/* jesin-default-terminal.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
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
static char *const MYPWD = "PWD=/home/jesin";
static char *const EXECPATH = "/usr/bin/termite";

static int spc(const void *a, const void *b) {
	return strcmp(*(const char *const *)a, *(const char *const *)b);
}

int main(int argc, char *const argv[]) {
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
	size_t i = 0;
	for (size_t k = 0; environ[i];) {
		++k;
		if (*(uint16_t*)environ[i] != *USCORE_U16 && *(uint32_t*)environ[i] != *(uint32_t*)MYPWD && (*(uint64_t*)environ[i] & *U64_7BMASK) != *OLDPWD_U64) {
			++i;
		}
		if (i != k) {
			environ[i] = environ[k];
		}
	}
	if (chdir(MYPWD + 4) >= 0) {
		environ[i] = MYPWD;
		environ[++i] = NULL;
	}
	qsort(environ, i, sizeof(char*), spc);
	char **args = malloc(sizeof(char*) * ((size_t)argc + 3));
	if (!args) {
		return toFailureCode(errno);
	}
	args[0] = EXECPATH + 9;
	args[1] = "-e";
	args[2] = "clenv tmux new -As0 -nlog journalctl -oshort-precise -fn8192";
	memcpy(&args[3], &argv[1], sizeof(char*) * (size_t)argc);
	execv(EXECPATH, args);
	free(args); args = NULL;
	return toFailureCode(errno);
}
