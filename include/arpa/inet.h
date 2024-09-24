#include <sys/cdefs.h>

#ifndef _ARPA_INET_H_
#define _ARPA_INET_H_

__BEGIN_DECLS

unsigned int htonl(unsigned int n);
unsigned int ntohl(unsigned int n);
unsigned short htons(unsigned short n);
unsigned short ntohs(unsigned short n);

__END_DECLS

#endif
