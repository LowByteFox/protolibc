#include <sys/cdefs.h>

#ifndef _SYS_UTSNAME_H_
#define _SYS_UTSNAME_H_

__BEGIN_DECLS

struct utsname {
	char sysname[65];
	char nodename[65];
	char release[65];
	char version[65];
	char machine[65];
    char __domainname[65];
};

int uname(struct utsname *buf);

__END_DECLS

#endif
