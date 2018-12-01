#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <errno.h>
#include <limits.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static inline int makeFailureCode(int e) {
	return (e & 255) ? e : (e | 248);
}

int main(int argc, const char *const *argv) {
	setpriority(PRIO_PROCESS, 0, INT_MAX/2);
	setpgid(0, 0);
	{
		struct sigaction x;
		memset(&x, 0, sizeof(x));
		x.sa_handler = SIG_DFL;
		x.sa_flags = SA_NOCLDSTOP | SA_NOCLDWAIT;
		if (sigaction(SIGCHLD, &x, NULL) < 0) {
			return makeFailureCode(errno);
		}
	}
	unsigned n = 0;
	{
		int i;
		if (argc == 2 && sscanf(argv[1], "%i", &i) > 0 && i >= 0) {
			n = i;
		}
	}
	if (!n) {
		fprintf(stderr,
			"Usage: %s n\n"
			"Creates a total of n processes (where n >= 1) and exits.\n",
			argv[0]
		);
		return makeFailureCode(errno);
	}
	--n;
	unsigned i = 4;
	while (n) {
		pid_t x = fork();
		if (x < 0) {
			int e = errno;
			if (e != EAGAIN || i <= 0) {
				fprintf(stderr, "Error: %s PID %jd failed to fork. Exiting with %d fork%s remaining. errno=%d (%s)\n", argv[0], (intmax_t)getpid(), n, (n == 1 ? "" : "s"), e, strerror(e));
				return makeFailureCode(e);
			}
			--i;
			wait(NULL);
			sched_yield();
			continue;
		}
		if (x == 0) {
			--n;
		}
		n >>= 1;
		i = 4;
	}
	return 0;
}
