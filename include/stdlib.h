#include <sys/cdefs.h>
#include <stddef.h>

#ifndef _STDLIB_H_
#define _STDLIB_H_

__BEGIN_DECLS

#define RAND_MAX		0x7fffffff

extern const char *malloc_options;

void *malloc(size_t size);
void *fastmalloc(size_t size);
size_t malloc_usable_size(void *ptr);

void *calloc(size_t nmemb, size_t size);
void *fastcalloc(size_t nmemb, size_t size);

void *realloc(void *ptr, size_t new_size);
void *fastrealloc(void *ptr, size_t new_size);
void *reallocarray(void *ptr, size_t nmemb, size_t size);
void *recallocarray(void *ptr, size_t oldnmemb, size_t nmemb, size_t size);

void free(void *ptr);
void freezero(void *ptr, size_t size);

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
