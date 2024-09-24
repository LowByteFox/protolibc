#include <sys/cdefs.h>

#ifndef _INTTYPES_H_
#define _INTTYPES_H_

__BEGIN_DECLS

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#ifdef __x86_64__
typedef unsigned long uint64_t;
typedef long int64_t;
#else
typedef unsigned long long uint64_t;
typedef long long int64_t;
#endif

__END_DECLS

#endif
