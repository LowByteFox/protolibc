#include <sys/cdefs.h>
#include <stddef.h>

#ifndef _STRING_H_
#define _STRING_H_

__BEGIN_DECLS

void *memcpy(void *dst, const void *src, size_t n);
void *rmemcpy(void *dst, const void *src, size_t n);
void *memmove(void *dst, const void *src, size_t n);
void *memset(void *s, int v, size_t n);
void *memchr(const void *s, int c, size_t n);
void *memrchr(void *s, int c, long n);
int memcmp(const void *s1, const void *s2, size_t n);

char *strcpy(char *dst, const char *src);
char *strchr(const char *s, int c);
char *strchrnul(const char *s, int ch, size_t n);
char *strrchr(const char *s, int ch);
size_t strlen(const char *s);
int strcmp(const char *s1, const char *s2);
char *stpcpy(char *dst, const char *srt);

size_t strspn(const char *s, const char *accept);
size_t strcspn(const char *s, const char *reject);
char *strpbrk(const char *s, const char *accept);
char *strsep(char **stringp, const char *delim);
char *strtok(char *s, const char *delim);

char *strncpy(char *d, const char *s, size_t n);
char *strcat(char *d, const char *s);
char *strncat(char *dest, const char *src, size_t n);
int strncmp(const char *s1, const char *s2, size_t n);
char *strstr(const char *s, const char *r);
char *strerror(int errnum);

char *strdup(const char *s);
char *strndup(const char *s, size_t n);

/* BSD related string function */
size_t strlcpy(char *dst, const char *src, size_t dstsize);
size_t strlcat(char *dst, const char *src, size_t dstsize);

__END_DECLS

#endif
