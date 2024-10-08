#include <sys/cdefs.h>
#include <stddef.h>

#ifndef _STRING_H_
#define _STRING_H_

__BEGIN_DECLS

void *memcpy(void *dst, void *src, long n);
void *memmove(void *dst, void *src, long n);
void *memset(void *s, int v, size_t n);
void *memchr(void *s, int c, long n);
void *memrchr(void *s, int c, long n);
int memcmp(char *s1, char *s2, long n);

char *strcpy(char *dst, char *src);
char *strchr(char *s, int c);
char *strrchr(char *s, int c);
size_t strlen(const char *s);
int strcmp(char *s1, char *s2);

char *strncpy(char *d, char *s, long n);
char *strcat(char *d, char *s);
int strncmp(char *d, char *s, long n);
char *strstr(char *s, char *r);

char *strdup(const char *s);

/* BSD related string function */
size_t strlcpy(char *dst, const char *src, size_t dstsize);
size_t strlcat(char *dst, const char *src, size_t dstsize);

/* faster implementations */
#define memchr(s, c, n)		__memchr_c(s, c, n)

void *__memchr_c(void *s, int c, long n);

__END_DECLS

#endif
