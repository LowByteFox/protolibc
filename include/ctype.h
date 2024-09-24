#include <sys/cdefs.h>

#ifndef	_CTYPE_H_
#define _CTYPE_H_

__BEGIN_DECLS

int isascii(int c);
int isblank(int c);
int isspace(int c);
int isalpha(int c);
int isdigit(int c);
int isalnum(int c);
int isupper(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);

int tolower(int c);
int toupper(int c);

__END_DECLS

#endif
