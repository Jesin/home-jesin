/* printSizeofStructStat.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

alignas(4096) static char buf[8192];

#define PRINT_SIZEOF_ALIGNOF(T) printf(" sizeof(%s): %#zx\nalignof(%s): %#zx\n", #T, sizeof(T), #T, alignof(T))
#define PRINT_OFFSETOF(T, M) printf("offsetof(%s, %s): %#zx\n", #T, #M, offsetof(T, M))

int main(void) {
	setvbuf(stdout, buf, _IOFBF, sizeof(buf));
	PRINT_SIZEOF_ALIGNOF(struct stat);
	putc('\n', stdout);
	PRINT_OFFSETOF(struct stat, st_mode);
	PRINT_SIZEOF_ALIGNOF(mode_t);
	putc('\n', stdout);
	PRINT_OFFSETOF(struct stat, st_size);
	PRINT_SIZEOF_ALIGNOF(off_t);
	return 0;
}
