#include <sys/cdefs.h>

#ifndef _ARENA_H_
#define _ARENA_H_

#include <stddef.h>

/*
 * allocated - total allocation will be used for next chunk
 * pos - how much was allocated
 */
struct arena {
    struct arena *next;
    void *data;
    size_t allocated, pos;
};

struct arena arena_init(size_t size);
void arena_reset(struct arena *arena);
void arena_deinit(struct arena *arena);

void *arena_alloc(struct arena *arena, size_t size);

#endif
