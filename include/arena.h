#include <sys/cdefs.h>

#ifndef _ARENA_H_
#define _ARENA_H_

#include <stddef.h>

__BEGIN_DECLS

struct arena {
    struct arena *next;
    void *data;
    size_t allocated, pos, count;
};

struct arena arena_init(size_t size);
void arena_reset(struct arena *arena);
void arena_deinit(struct arena *arena);

void *arena_alloc(struct arena *arena, size_t size);

__END_DECLS

#endif
