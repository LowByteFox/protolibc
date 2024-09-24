#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void __assert(const char *file, int line, const char *fn, const char *expr)
{
    fprintf(stderr,
        "assertion \"%s\" failed: %s@%s#%d\n", expr, file, fn, line);

    abort();
}
