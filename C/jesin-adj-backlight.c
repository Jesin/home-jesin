/* jesin-adj-backlight.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <errno.h>
#include <fcntl.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define JESBUFSIZE (4096)
#define JESBUFALIGN (4096)

alignas(JESBUFALIGN) char buf[JESBUFSIZE];

static inline int makeFailureCode(int e) {
	return (e & 255) ? e : (e | 128);
}

int main(int argc, char const *const *argv) {
	intmax_t inc;
	if (argc != 3 || sscanf(argv[2], "%ji", &inc) != 1) { return makeFailureCode(EINVAL); }
	if ((*argv[1] != '/' && chdir("/sys/class/backlight") < 0) || chdir(argv[1]) < 0) { return makeFailureCode(errno); }
	int fd = open("max_brightness", O_RDONLY);
	if (fd < 0) { return makeFailureCode(errno); }
	ssize_t len = read(fd, buf, JESBUFSIZE);
	if (len <= 0 || close(fd) < 0) { return makeFailureCode(errno); }
	/* NOTE: fd was just closed. */
	if (len > JESBUFSIZE-1) { len = JESBUFSIZE-1; }
	buf[len] = 0;
	intmax_t bmax;
	if (sscanf(buf, "%ji", &bmax) != 1) { return makeFailureCode(errno); }
	fd = open("brightness", O_RDWR);
	if (fd < 0) { return makeFailureCode(errno); }
	len = read(fd, buf, JESBUFSIZE);
	if (len <= 0) { return makeFailureCode(errno); }
	if (len > JESBUFSIZE-1) { len = JESBUFSIZE-1; }
	buf[len] = 0;
	intmax_t b;
	if (sscanf(buf, "%ji", &b) != 1) { return makeFailureCode(errno); }
	b += inc;
	if (b < 0) { b = 0; }
	if (b > bmax) { b = bmax; }
	len = snprintf(buf, JESBUFSIZE, "%ji", b);
	if (len <= 0 || len >= JESBUFSIZE || write(fd, buf, len) != len || close(fd) < 0) { return makeFailureCode(errno); }
	/* NOTE: fd was just closed. */
	return 0;
}
