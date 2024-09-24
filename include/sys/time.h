#include <sys/cdefs.h>

#ifndef _SYS_TIME_H_
#define _SYS_TIME_H_

__BEGIN_DECLS

struct timeval {
	long tv_sec;
	long tv_usec;
};

struct timezone {
	int tz_minuteswest;
	int tz_dsttime;
};

int gettimeofday(struct timeval *tv, struct timezone *tz);

__END_DECLS

#endif
