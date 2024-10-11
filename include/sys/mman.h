#include <stddef.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#ifndef _SYS_MMAN_H_
#define _SYS_MMAN_H_

__BEGIN_DECLS

#define PROT_NONE	0x0
#define PROT_READ	0x1
#define PROT_WRITE	0x2
#define PROT_EXEC	0x4

#define MAP_SHARED	0x01
#define MAP_PRIVATE	0x02

#define MAP_FIXED	0x10		/* Interpret addr exactly */
#define MAP_ANONYMOUS	0x20		/* don't use a file */
#define MAP_GROWSDOWN	0x0100		/* stack-like segment */
#define MAP_DENYWRITE	0x0800		/* ETXTBSY */
#define MAP_EXECUTABLE	0x1000		/* mark it as an executable */
#define MAP_LOCKED	0x2000		/* pages are locked */
#define MAP_NORESERVE	0x4000		/* don't check for reservations */
#define MAP_POPULATE	0x8000
#define MAP_ANON	MAP_ANONYMOUS
#define MAP_FILE	0

#define MAP_FAILED      ((void *) -1)

void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
int mprotect(void *addr, long len, int prot);
int munmap(void *addr, size_t len);

__END_DECLS

#endif
