/* printrlimits.c by Kevin Dodd */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <errno.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>

#define JESBUFSIZE BUFSIZ
#define JESBUFALIGN (4096)

alignas(JESBUFALIGN) char buf[JESBUFSIZE];

static int print_rlimit(int resource, const char *resource_name)
{
	struct rlimit rlim;
	int rcode = getrlimit(resource, &rlim);
	if (rcode) {
		printf("Warning: call to getrlimit(RLIMIT_%s, &rlim) returned %d (errno=%d)\n", resource_name, rcode, errno);
	}
	return printf("%4d %11s %#19jx %#19jx %#19jx %12.6g %12.6g\n", resource, resource_name, (uintmax_t)rlim.rlim_cur, (uintmax_t)rlim.rlim_max, (uintmax_t)(rlim.rlim_max-rlim.rlim_cur), (double)rlim.rlim_cur / (double)rlim.rlim_max, (double)rlim.rlim_max / (double)rlim.rlim_cur);
}

#define PRINT_RLIMIT(name) print_rlimit(RLIMIT_##name, #name)

int main(void)
{
	setvbuf(stdout, buf, _IOFBF, JESBUFSIZE);
	printf(
		"sizeof(rlim_t): %zu\n"
		"PAGESIZE (%ju): %lu\n"
		"\n"
		"RLIM_NLIMITS   = %3ju\n"
		"RLIM_INFINITY  = %#19jx\n"
		"\n"
		"  #         Name          Soft Limit          Hard Limit           Hard-Soft    Soft/Hard    Hard/Soft\n",
		sizeof(rlim_t),
		(uintmax_t)_SC_PAGESIZE, (unsigned long)sysconf(_SC_PAGESIZE),
		(uintmax_t)RLIM_NLIMITS, (uintmax_t)RLIM_INFINITY);
	PRINT_RLIMIT(CPU);
	PRINT_RLIMIT(FSIZE);
	PRINT_RLIMIT(DATA);
	PRINT_RLIMIT(STACK);
	PRINT_RLIMIT(CORE);
#ifdef RLIMIT_RSS
	PRINT_RLIMIT(RSS);
#endif
#ifdef RLIMIT_NPROC
	PRINT_RLIMIT(NPROC);
#endif
	PRINT_RLIMIT(NOFILE);
#ifdef RLIMIT_MEMLOCK
	PRINT_RLIMIT(MEMLOCK);
#endif
	PRINT_RLIMIT(AS);
#ifdef RLIMIT_LOCKS
	PRINT_RLIMIT(LOCKS);
#endif
#ifdef RLIMIT_SIGPENDING
	PRINT_RLIMIT(SIGPENDING);
#endif
#ifdef RLIMIT_MSGQUEUE
	PRINT_RLIMIT(MSGQUEUE);
#endif
#ifdef RLIMIT_NICE
	PRINT_RLIMIT(NICE);
#endif
#ifdef RLIMIT_RTPRIO
	PRINT_RLIMIT(RTPRIO);
#endif
#ifdef RLIMIT_RTTIME
	PRINT_RLIMIT(RTTIME);
#endif
	return 0;
}
