#include <stdint.h>
#include <string.h>

void *memcpy(void *dst, const void *src, size_t n)
{
    uint8_t *d = dst;
    const uint8_t *s = src;

    while (n--) {
        *d++ = *s++;
    }

    return dst;
}

void *memmove(void *dst, const void *src, size_t n)
{
    uint8_t *d = dst;
    const uint8_t *s = src;

    if (d < s) {
        while (n--) {
            *d++ = *s++;
        }
    } else {
        d += n;
        s += n;
        while (n--) {
            *--d = *--s;
        }
    }

    return dst;
}

void *memset(void *s, int v, size_t n)
{
    uint8_t* p = s;

    while (n--) {
        *p++ = (uint8_t) v;
    }

    return s;
}

void *memrchr(void *s, int c, long n)
{
    const uint8_t *p;

    if (n != 0) {
        p = ((uint8_t*) s) + n;
        do {
            if (*(--p) == (uint8_t) c)
                return (void*) p;
        } while (--n != 0);
    }

    return NULL;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    const uint8_t *p1 = s1;
    const uint8_t *p2 = s2;

    while (n--) {
        if (*p1 != *p2) {
            return *p1 - *p2;
        }

        p1++;
        p2++;
    }

    return 0;
}

char *strcpy(char *dst, const char *src)
{
    char *start = dst;

    for (; (*dst = *src); ++dst, ++src);

    return start;
}

char *strchr(const char *s, int c)
{
    while (*s) {
        if (*s == c) {
            return (char*) s;
        }
        s++;
    }

    if (c == 0) {
        return (char*) s;
    }

    return NULL;
}

char *strrchr(const char *s, int ch)
{
    char c, *save;

    c = ch;
    for (save = NULL;; ++s) {
        if (*s == c)
            save = (char *) s;
        if (*s == 0)
            return save;
    }
}

size_t strlen(const char *s)
{
    size_t length = 0;
    while (*s) {
        length++;
        s++;
    }

    return length;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 != *s2++)
        if (*s1++ == 0)
            return 0;

    return (*(const uint8_t*)s1 - *(const uint8_t*)(s2 - 1));
}

int strncmp(const char *s1, const char *s2, size_t n)
{
    if (n == 0)
        return 0;

    do {
        if (*s1 != *s2++)
            return *(uint8_t*) s1 - *(uint8_t*) --s2;
        if (*s1++ == 0)
            break;
    } while (--n != 0);

    return 0;
}

size_t strspn(const char *s, const char *accept)
{
    const char *p = s, *spanp;
    char c, sc;
    
cont:
    c = *p++;
    for (spanp = accept; (sc = *spanp++) != 0;)
        if (sc == c)
            goto cont;

    return p - 1 - s;
}

size_t strcspn(const char *s, const char *reject)
{
	const char *p, *spanp;
	char c, sc;

	for (p = s;;) {
		c = *p++;
		spanp = reject;
		do {
			if ((sc = *spanp++) == c)
				return p - 1 - s;
		} while (sc != 0);
	}
}

char *strerror(int errnum)
{
    return NULL;
}

size_t strlcpy(char *dst, const char *src, size_t dstsize)
{
    const char *osrc = src;
    size_t nleft = dstsize;

    /* Copy as many bytes as will fit. */
    if (nleft != 0) {
        while (--nleft != 0) {
            if ((*dst++ = *src++) == '\0')
                break;
        }
    }

    /* Not enough room in dst, add NUL and traverse rest of src. */
    if (nleft == 0) {
        if (dstsize != 0)
            *dst = '\0'; /* NUL-terminate dst */
        while (*src++)
            ;
    }

    return src - osrc - 1; /* count does not include NUL */
}

size_t strlcat(char *dst, const char *src, size_t dstsize)
{
    const char *odst = dst;
    const char *osrc = src;
    size_t n = dstsize;
    size_t dlen;

    /* Find the end of dst and adjust bytes left but don't go past end. */
    while (n-- != 0 && *dst != '\0')
        dst++;
    dlen = dst - odst;
    n = dstsize - dlen;

    if (n-- == 0)
        return(dlen + strlen(src));

    while (*src != '\0') {
        if (n != 0) {
            *dst++ = *src;
            n--;
        }
        src++;
    }

    *dst = '\0';

    return dlen + (src - osrc); /* count does not include NUL */
}
