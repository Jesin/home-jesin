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
#define JESBUFSZ (0x40000)
#define JESOUTPIPESZ (0x40000)
#define JESSPLICELEN (0x7fffffff&-HUGE_PAGE_SIZE)
#define JESSENDFILELEN JESSPLICELEN

static inline int toExitCode(int e) {
	return (e && !(e & 255)) ? (e | 248) : e;
}

const char *const defaultArgv[] = {"", NULL};

alignas(PAGE_SIZE) char mybuf[JESBUFSZ];

int err;

static int widenPipe(int fd) {
	int sz = fcntl(fd, F_GETPIPE_SZ);
	if (sz >= 0 && sz < JESOUTPIPESZ) {
		sz = fcntl(fd, F_SETPIPE_SZ, JESOUTPIPESZ);
	}
	return sz;
}

static void rwloop(int fd) {
	while (1) {
		char *buf = mybuf;
		ssize_t r = read(fd, buf, JESBUFSZ);
		if (r <= 0) {
			if (r < 0 && !err) { err = errno; }
			return;
		}
		do {
			ssize_t w = write(fd, buf, r);
			if (w <= 0) {
				if (!err) { err = errno; }
				return;
			}
			buf += w;
			r -= w;
		} while (r > 0);
	}
}

int main(int argc, const char *const *argv) {
	(void)argc; /* suppress "unused" warning */
	if (!*++argv) {
		argv = defaultArgv;
	}
	widenPipe(STDOUT_FILENO);
	int internalPipe[2] = {-1, -1};
	errno = err = 0;
	do {
		int infd;
		if (**argv) {
			infd = open(*argv, O_RDONLY);
			if (infd < 0) {
				if (!err) { err = errno; }
				continue;
			}
		} else {
			infd = STDIN_FILENO;
		}
		if (posix_fadvise(infd, 0, 0, POSIX_FADV_SEQUENTIAL) >= 0) {
			posix_fadvise(infd, 0, 0, POSIX_FADV_WILLNEED);
		}

		size_t sz;
		{
			struct stat statbuf;
			if (fstat(infd, &statbuf) >= 0) {
				sz = statbuf.st_size;
			} else {
				sz = 0;
			}
		}

		while (sz) {
			ssize_t r = copy_file_range(infd, NULL, STDOUT_FILENO, NULL, sz, 0);
			if (r <= 0) { break; }
			sz -= r;
			if (!sz) { goto CloseAndContinue; }
		}

		while (1) {
			ssize_t r = sendfile(STDOUT_FILENO, infd, NULL, JESSENDFILELEN);
			if (r == 0) { goto CloseAndContinue; }
			if (r < 0) { break; }
		}

		while (1) {
			ssize_t r = splice(infd, NULL, STDOUT_FILENO, NULL, JESSPLICELEN, SPLICE_F_MORE);
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
				ssize_t r = splice(infd, NULL, internalPipe[1], NULL, JESSPLICELEN, SPLICE_F_MORE);
				if (r == 0) { goto CloseAndContinue; }
				if (r < 0) { break; }
				do {
					ssize_t w = splice(internalPipe[0], NULL, STDOUT_FILENO, NULL, r, SPLICE_F_MORE);
					if (w <= 0) { break; }
					r -= w;
				} while (r > 0);
				if (r > 0) {
					if (close(internalPipe[1]) < 0 && !err) { err = errno; }
					rwloop(internalPipe[0]);
					if (close(internalPipe[0]) < 0 && !err) { err = errno; }
					internalPipe[1] = internalPipe[0] = -1;
					break;
				}
			}
		}

		rwloop(infd);

CloseAndContinue:
		if (infd != STDIN_FILENO && close(infd) < 0 && !err) {
			err = errno;
		}
	} while (*++argv);
	return toExitCode(err);
}
