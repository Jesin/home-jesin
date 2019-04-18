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

static const uint64_t *const U64_5BMASK = (const uint64_t *)"\xFF\xFF\xFF\xFF\xFF\0\0";
static const uint64_t *const U64_7BMASK = (const uint64_t *)"\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
static const uint64_t *const HOME_U64 = (const uint64_t *)"HOME=\0\0";
static const uint64_t *const OLDPWD_U64 = (const uint64_t *)"OLDPWD=";
static const uint32_t *const PWD_U32 = (const uint32_t *)"PWD=";
static const uint16_t *const USCORE_U16 = (const uint16_t *)"_=";
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
	char *home = NULL;
	char *pwd = NULL;
	size_t i = 0;
	for (size_t k = 0; environ[i];) {
		++k;
		if (*(uint32_t*)environ[i] == *PWD_U32) {
			pwd = environ[i];
		} else if (*(uint16_t*)environ[i] != *USCORE_U16 && (*(uint64_t*)environ[i] & *U64_7BMASK) != *OLDPWD_U64) {
			if ((*(uint64_t*)environ[i] & *U64_5BMASK) == *HOME_U64) {
				home = environ[i] + 5;
			}
			++i;
		}
		if (i != k) {
			environ[i] = environ[k];
		}
	}
	if (!home) {
		home = "/home/jesin";
	}
	if (chdir(home) >= 0) {
		size_t homelen = strlen(home);
		if (!pwd || strnlen(pwd + 4, homelen) < homelen) {
			pwd = malloc(homelen + 5);
			if (!pwd) {
				return toFailureCode(errno);
			}
			*(uint32_t*)pwd = *PWD_U32;
		}
		memcpy(pwd + 4, home, homelen + 1);
		environ[i] = pwd;
		environ[++i] = NULL;
	}
	qsort(environ, i, sizeof(char*), spc);
	char **args = malloc(sizeof(char*) * ((size_t)argc + 3));
	if (!args) {
		return toFailureCode(errno);
	}
	args[0] = EXECPATH + 9;
	args[1] = "-e";
	args[2] = "cle tmux new -As0 -nlog journalctl -oshort-precise -fn8192";
	memcpy(&args[3], &argv[1], sizeof(char*) * (size_t)argc);
	execv(EXECPATH, args);
	free(args); args = NULL;
	return toFailureCode(errno);
}
