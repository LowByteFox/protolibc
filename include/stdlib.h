#include <sys/cdefs.h>
#include <stddef.h>

#ifndef _STDLIB_H_
#define _STDLIB_H_

__BEGIN_DECLS

#define RAND_MAX		0x7fffffff

void *malloc(size_t n);
void *fastmalloc(size_t n);
size_t malloc_usable_size(void *ptr);

void free(void *m);
void *calloc(size_t n, size_t sz);
void *realloc(void *v, size_t sz);

int atoi(char *s);
long atol(char *s);
long strtol(const char *s, char **endptr, int base);
unsigned long strtoul(const char *s, char **endptr, int base);
int abs(int n);
long labs(long n);

void exit(int status);
void abort(void);
int atexit(void (*func)(void));

char *getenv(char *name);
void qsort(void *a, int n, int sz, int (*cmp)(void *, void *));
int mkstemp(char *t);
int system(char *cmd);

void srand(unsigned int seed);
int rand(void);

const char *getprogname();
void setprogname(const char *);

__END_DECLS

#endif
