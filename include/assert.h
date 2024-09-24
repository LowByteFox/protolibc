#include <sys/cdefs.h>

/* 
 * Unlike other ANSI headers, <assert.h> may be included multiple times,
 * with and without NDEBUG defined
 */

#undef assert
#undef _assert

/*
 * proot specific assert macros
 * assert2 - if NDEBUG isn't defined, and PROTO_DEBUG env is defined, assert
 * env_assert - if PROTO_DEBUG env is defined, assert
 */

#undef assert2
#undef env_assert

#ifdef NDEBUG
# define assert(e) ((void) 0)
# define _assert(e) ((void) 0)
# define assert2(e) ((void) 0)
#else
# define _assert(e) assert(e)
# define assert2(e) env_assert(e)
# define assert(e) ((e) ? (void)0 : __assert(__FILE__, __LINE__, __func__, #e))
#endif

#define env_assert(e) do {                          \
        if (getenv("PROTO_DEBUG") != (void*) 0)     \
            assert(e);                              \
        } while (0)                                 \

#ifndef _ASSERT_H_
#define _ASSERT_H_

__BEGIN_DECLS

void __assert(const char *, int, const char *, const char *);

__END_DECLS

#endif
