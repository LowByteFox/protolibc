/* https://repo.or.cz/tinycc.git/blob/HEAD:/include/stdarg.h */
#include <sys/cdefs.h>

#ifndef _STDARG_H_
#define _STDARG_H_

__BEGIN_DECLS

typedef __builtin_va_list va_list;
#define va_start __builtin_va_start
#define va_arg __builtin_va_arg
#define va_copy __builtin_va_copy
#define va_end __builtin_va_end

/* fix a buggy dependency on GCC in libio.h */
typedef va_list __gnuc_va_list;
#define _VA_LIST_DEFINED

__END_DECLS

#endif
