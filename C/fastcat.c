/* fastcat.c by Kevin Dodd */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <stdalign.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef PAGE_SHIFT
#define PAGE_SHIFT (12)
#endif
#ifndef PAGE_SIZE
#define PAGE_SIZE (1<<PAGE_SHIFT)
#endif
#ifndef HUGE_PAGE_SHIFT
#define HUGE_PAGE_SHIFT (21)
#endif
#ifndef HUGE_PAGE_SIZE
#define HUGE_PAGE_SIZE (1<<HUGE_PAGE_SHIFT)
#endif
#define JESOUTPIPESZ (0x40000)
#define JESSPLICELEN (0x7fffffff&-HUGE_PAGE_SIZE)
#define JESSENDFILELEN JESSPLICELEN

static inline int toExitCode(int e) {
	return (e && !(e & 255)) ? (e | 248) : e;
}

alignas(PAGE_SIZE) static char buf[0x40000];
static int err;

static int widenPipe(int fd) {
	int sz = fcntl(fd, F_GETPIPE_SZ);
	if (sz >= 0 && sz < JESOUTPIPESZ) {
		sz = fcntl(fd, F_SETPIPE_SZ, JESOUTPIPESZ);
	}
	return sz;
}

static void rwloop(int fd) {
	while (1) {
		char *a = buf;
		ssize_t r = read(fd, a, sizeof buf);
		if (r < 0) { err = errno; }
		if (r <= 0) { return; }
		do {
			ssize_t w = write(1, a, r);
			if (w < 0) { err = errno; }
			if (w <= 0) { return; }
			a += w;
			r -= w;
		} while (r > 0);
	}
}

int main(int argc, char **argv) {
	(void)argc; /* suppress "unused" warning */
	if (!*++argv) {
		**--argv = 0;
	}
	widenPipe(1);
	int internalPipe[2] = {-1, -1};
	errno = err = 0;
	do {
		int fd;
		if (**argv) {
			fd = open(*argv, O_RDONLY);
			if (fd < 0) {
				err = errno;
				continue;
			}
		} else {
			fd = 0;
		}

		size_t sz;
		{
			struct stat s;
			sz = fstat(fd, &s) < 0 ? 0 : s.st_size;
		}

		if (posix_fadvise(fd, 0, sz, POSIX_FADV_SEQUENTIAL) >= 0) {
			posix_fadvise(fd, 0, sz, POSIX_FADV_WILLNEED);
		}

		if (sz) while (1) {
			ssize_t r = copy_file_range(fd, NULL, 1, NULL, sz, 0);
			if (r <= 0) { break; }
			sz -= r;
			if (!sz) { goto CloseAndContinue; }
		}

		while (1) {
			ssize_t r = sendfile(1, fd, NULL, JESSENDFILELEN);
			if (r == 0) { goto CloseAndContinue; }
			if (r < 0) { break; }
		}

		while (1) {
			ssize_t r = splice(fd, NULL, 1, NULL, JESSPLICELEN, SPLICE_F_MORE);
			if (r == 0) { goto CloseAndContinue; }
			if (r < 0) { break; }
		}

		if (internalPipe[1] < 0) {
			if (pipe(internalPipe) < 0) {
				internalPipe[1] = internalPipe[0] = -1;
			} else {
				widenPipe(internalPipe[1]);
			}
		}
		if (internalPipe[1] >= 0) {
			while (1) {
				ssize_t r = splice(fd, NULL, internalPipe[1], NULL, JESSPLICELEN, SPLICE_F_MORE);
				if (r == 0) { goto CloseAndContinue; }
				if (r < 0) { break; }
				do {
					ssize_t w = splice(internalPipe[0], NULL, 1, NULL, r, SPLICE_F_MORE);
					if (w <= 0) { break; }
					r -= w;
				} while (r > 0);
				if (r > 0) {
					if (close(internalPipe[1]) < 0) { err = errno; }
					rwloop(internalPipe[0]);
					if (close(internalPipe[0]) < 0) { err = errno; }
					internalPipe[1] = internalPipe[0] = -1;
					break;
				}
			}
		}

		rwloop(fd);

CloseAndContinue:
		if (fd && close(fd) < 0) {
			err = errno;
		}
	} while (*++argv);
	return toExitCode(err);
}
