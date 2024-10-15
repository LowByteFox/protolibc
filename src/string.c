#include <string.h>

#define LONG01	(0x01010101ul | (0x01010101ul << (sizeof(long) * 8 - 32)))
#define LONG80	(0x80808080ul | (0x80808080ul << (sizeof(long) * 8 - 32)))
#define LONGOFF(s)	((unsigned long) s & (sizeof(long) - 1))
#define HASZERO(x)	(((x) - LONG01) & ~(x) & LONG80)

void *__memchr_c(void *src, int c, long n)
{
	unsigned char *s = src;
	c = (unsigned char) c;
	unsigned long k = LONG01 * c;
	while (LONGOFF(s) && n && *s != c)
		s++, n--;
	if (n && *s != c) {
		unsigned long *w = (void *) s;
		while (n >= sizeof(long) && !HASZERO(*w ^ k))
			w++, n -= sizeof(long);
		s = (void *) w;
	}
	while (n && *s != c)
		s++, n--;
	return n ? (void *) s : 0;
}

void *memcpy(void *dst, void *src, long n) {
    unsigned char *dst_ptr = (unsigned char *)dst;
    const unsigned char *src_ptr = (const unsigned char *)src;

    while (n--) {
        *dst_ptr++ = *src_ptr++;
    }

    return dst;
}

void *memmove(void *dst, void *src, long n) {
    unsigned char *dst_ptr = (unsigned char *)dst;
    const unsigned char *src_ptr = (const unsigned char *)src;

    if (src_ptr < dst_ptr && src_ptr + n > dst_ptr) {
        dst_ptr += n;
        src_ptr += n;
        while (n--) {
            *--dst_ptr = *--src_ptr;
        }
    } else {
        while (n--) {
            *dst_ptr++ = *src_ptr++;
        }
    }

    return dst;
}

void *memset(void *s, int v, size_t n) {
    unsigned char* p = (unsigned char*)s;

    while (n--) {
        *p++ = (unsigned char)v;
    }
    return s;
}

void *memrchr(void *s, int c, long n) {
    unsigned char *p = (unsigned char *)s + n - 1;
    while (n-- > 0) {
        if (*p == (unsigned char)c) {
            return (void *)p;
        }
        p--;
    }
    return NULL;
}

int memcmp(char *s1, char *s2, long n) {
    unsigned char *p1 = (unsigned char *)s1;
    unsigned char *p2 = (unsigned char *)s2;
    while (n-- > 0) {
        if (*p1 != *p2) {
            return *p1 - *p2;
        }
        p1++;
        p2++;
    }
    return 0;
}

char *strcpy(char *dst, char *src) {
    char *start = dst;
    while (*src != '\0') {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = '\0';
    return start;
}

char *strchr(const char *s, int c) {
    while (*s != '\0') {
        if (*s == c) {
            return (char*) s;
        }
        s++;
    }
    if (c == '\0') {
        return (char*) s;
    }
    return NULL;
}

char *strrchr(char *s, int c) {
    char *last = NULL;
    while (*s != '\0') {
        if (*s == c) {
            last = s;
        }
        s++;
    }
    if (c == '\0') {
        return s;
    }
    return last;
}

size_t strlen(const char *s) {
    size_t length = 0;
    while (*s != '\0') {
        length++;
        s++;
    }
    return length;
}

int strcmp(char *s1, char *s2) {
    while (*s1 != '\0' && *s2 != '\0') {
        if (*s1 < *s2) {
            return -1;
        } else if (*s1 > *s2) {
            return 1;
        }
        s1++;
        s2++;
    }
    if (*s1 == '\0' && *s2 == '\0') {
        return 0;
    } else if (*s1 == '\0') {
        return -1;
    } else {
        return 1;
    }
}

int strncmp(char *d, char *s, long n) {
    while (n > 0 && *d != '\0' && *s != '\0') {
        if (*d < *s) {
            return -1;
        } else if (*d > *s) {
            return 1;
        }
        d++;
        s++;
        n--;
    }
    if (n > 0) {
        if (*d == '\0') {
            return -1;
        } else if (*s == '\0') {
            return 1;
        }
    }
    return 0;
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
