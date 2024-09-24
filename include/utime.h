#include <sys/cdefs.h>

#ifndef	_UTIME_H_
#define	_UTIME_H_

__BEGIN_DECLS

#include <sys/types.h>

struct utimbuf {
	time_t actime;
	time_t modtime;
};

int utime(char *path, struct utimbuf *times);

__END_DECLS

#endif
