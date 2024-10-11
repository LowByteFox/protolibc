#include <stdint.h>
#include <sys/cdefs.h>

#ifndef _SYS_TYPES_H_
#define _SYS_TYPES_H_

__BEGIN_DECLS

#include <inttypes.h>

typedef uint32_t dev_t;
typedef uint16_t gid_t;
typedef uint16_t mode_t;
typedef uint16_t nlink_t;
typedef uint16_t uid_t;

typedef int32_t pid_t;
typedef int64_t ssize_t;
typedef long time_t;
typedef long useconds_t;

typedef signed long off_t;
typedef signed long ino_t;
typedef uint32_t uid32_t;
typedef uint32_t gid32_t;
typedef long fpos_t;

__END_DECLS

#endif
