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

void *memset(void *s, int v, long n) {
    unsigned char *ptr = (unsigned char *)s;
    unsigned char byte = (unsigned char)v;

    while (n--) {
        *ptr++ = byte;
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

char *strchr(char *s, int c) {
    while (*s != '\0') {
        if (*s == c) {
            return s;
        }
        s++;
    }
    if (c == '\0') {
        return s;
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

long strlen(char *s) {
    long length = 0;
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
