#include <sys/cdefs.h>

#ifndef _STDDEF_H_
#define _STDDEF_H_

__BEGIN_DECLS

#define NULL			((void *) 0)
#define offsetof(type, field)	((int) (&((type *) 0)->field))

typedef unsigned long size_t;

__END_DECLS

#endif
