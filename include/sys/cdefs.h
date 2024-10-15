#ifndef	_SYS_CDEFS_H_
#define	_SYS_CDEFS_H_

#ifdef __cplusplus
# define __BEGIN_DECLS extern "C" {
# define __END_DECLS }
#else
# define __BEGIN_DECLS
# define __END_DECLS
#endif

#define __PROTOGEN__

#ifdef _GNU_SOURCE
#error "Who do you take me for? anti-POSIX?"
#endif

#ifdef __TINYC__
# define __unused __attribute__((__unused__))
# define __section(x) __attribute__((section(x)))
# define __aligned(x) __attribute__((aligned(x)))
# define __packed __attribute__((packed))
#endif

#endif
