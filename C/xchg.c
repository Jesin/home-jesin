/* xchg.c by Kevin Dodd */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 200809L
#endif
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main(int argc, const char *const *argv) {
	const char *prog = *argv;
	++argv;
	if (argc < 3 || !(argc & 1)) {
		fprintf(stderr, "Usage: %s SOURCE DEST [SOURCE DEST [...]]\n", prog);
		return EINVAL;
	}
	int e = 0;
	do {
		if (renameat2(AT_FDCWD, *argv, AT_FDCWD, argv[1], RENAME_EXCHANGE) < 0) {
			e = errno;
			const char *s = strerror(e);
			fprintf(stderr, "%s: cannot exchange '%s' with '%s': %d %s\n", prog, *argv, argv[1], e, s);
		}
		argv += 2;
	} while (*argv);
	return (e && !(e & 255)) ? (e | 248) : e;
}
