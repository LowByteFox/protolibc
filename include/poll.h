#include <sys/cdefs.h>

#ifndef _POLL_H_
#define _POLL_H_

__BEGIN_DECLS

#define POLLIN		0x0001
#define POLLPRI		0x0002
#define POLLOUT		0x0004
#define POLLERR		0x0008
#define POLLHUP		0x0010
#define POLLNVAL	0x0020
#define POLLRDNORM	0x0040
#define POLLRDBAND	0x0080
#define POLLWRBAND	0x0200
#define POLLMSG		0x0400
#define POLLREMOVE	0x1000

struct pollfd {
	int fd;
	short events;
	short revents;
};

typedef unsigned int nfds_t;

extern int poll(struct pollfd *ufds, nfds_t nfds, int timeout);

__END_DECLS

#endif
