#include <sys/cdefs.h>

#ifndef _SETJMP_H_
#define _SETJMP_H_

__BEGIN_DECLS

typedef long jmp_buf[8];

int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int val);

__END_DECLS

#endif 
