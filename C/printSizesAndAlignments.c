/* printSizesAndAlignments.c by Kevin Dodd */
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 201112L
#warning "This program relies on some features that did not appear in the C standard until C11."
#endif
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <time.h>

#define JESBUFSIZE BUFSIZ
#define JESBUFALIGN (4096)

alignas(JESBUFALIGN) char buf[JESBUFSIZE];

#define PRINT_SIZEOF_ALIGNOF(T) printf(" sizeof(%s): %#zx\nalignof(%s): %#zx\n", #T, (size_t)sizeof(T), #T, (size_t)alignof(T))
int main(void) {
	setvbuf(stdout, buf, _IOFBF, JESBUFSIZE);
	PRINT_SIZEOF_ALIGNOF(bool);
	PRINT_SIZEOF_ALIGNOF(char);
	PRINT_SIZEOF_ALIGNOF(unsigned char);
	PRINT_SIZEOF_ALIGNOF(signed char);
	PRINT_SIZEOF_ALIGNOF(short);
	PRINT_SIZEOF_ALIGNOF(int);
	PRINT_SIZEOF_ALIGNOF(long);
	PRINT_SIZEOF_ALIGNOF(long long);
	PRINT_SIZEOF_ALIGNOF(uint_fast8_t);
	PRINT_SIZEOF_ALIGNOF(uint_fast16_t);
	PRINT_SIZEOF_ALIGNOF(uint_fast32_t);
	PRINT_SIZEOF_ALIGNOF(uint_fast64_t);
	PRINT_SIZEOF_ALIGNOF(uint_least8_t);
	PRINT_SIZEOF_ALIGNOF(uint_least16_t);
	PRINT_SIZEOF_ALIGNOF(uint_least32_t);
	PRINT_SIZEOF_ALIGNOF(uint_least64_t);
	PRINT_SIZEOF_ALIGNOF(uintmax_t);
	PRINT_SIZEOF_ALIGNOF(void*);
	PRINT_SIZEOF_ALIGNOF(void(*)());
	PRINT_SIZEOF_ALIGNOF(size_t);
	PRINT_SIZEOF_ALIGNOF(ssize_t);
	PRINT_SIZEOF_ALIGNOF(ptrdiff_t);
	PRINT_SIZEOF_ALIGNOF(intptr_t);
	PRINT_SIZEOF_ALIGNOF(uintptr_t);
	PRINT_SIZEOF_ALIGNOF(pid_t);
	PRINT_SIZEOF_ALIGNOF(uid_t);
	PRINT_SIZEOF_ALIGNOF(gid_t);
	PRINT_SIZEOF_ALIGNOF(id_t);
	PRINT_SIZEOF_ALIGNOF(float);
	PRINT_SIZEOF_ALIGNOF(double);
	PRINT_SIZEOF_ALIGNOF(long double);
	PRINT_SIZEOF_ALIGNOF(max_align_t);
	PRINT_SIZEOF_ALIGNOF(struct { int a; });
	PRINT_SIZEOF_ALIGNOF(struct { size_t a; bool b; });
	PRINT_SIZEOF_ALIGNOF(struct { size_t a; char b; });
	PRINT_SIZEOF_ALIGNOF(struct { size_t a; unsigned b; });
	PRINT_SIZEOF_ALIGNOF(struct { max_align_t a; char b; });
	PRINT_SIZEOF_ALIGNOF(struct { char a; max_align_t b; });
	PRINT_SIZEOF_ALIGNOF(struct itimerspec);
	PRINT_SIZEOF_ALIGNOF(struct sigaction);
	PRINT_SIZEOF_ALIGNOF(struct termios);
	PRINT_SIZEOF_ALIGNOF(struct timespec);
	PRINT_SIZEOF_ALIGNOF(struct tm);
	PRINT_SIZEOF_ALIGNOF(FILE);
	return 0;
}
