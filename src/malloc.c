#include <stddef.h>
#include <stdbool.h>
#include <arena.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define PGSIZE 4096
#define align(x, align) (((x) + (align) - 1) & ~((align) - 1))

#define MEDIUM_SIZE 4096
#define LARGE_SIZE  8192

struct malloc_header {
    ssize_t usable_size;
};

struct huge_alloc {
    struct huge_alloc *next;
    size_t usable_size;
};

struct huge_alloc_pool {
    struct huge_alloc *first;
    struct huge_alloc *tail;
    size_t size;
};

enum pools {
    MEDIUM,
    LARGE,
    HUGE
};

/* context vars */
static bool initialized = false;

/* alloc pools */
static struct arena medium_pool;
static struct arena large_pool;
static struct huge_alloc_pool huge_pool;

/* functions */
static void malloc_init();
static bool is_pointer_valid(void *ptr);
static void *find_free_block(size_t size, enum pools pool);

void *malloc(size_t size)
{
    if (!initialized)
        malloc_init();

    if (size < MEDIUM_SIZE) {
        void *block = find_free_block(size, MEDIUM);
        if (block == NULL)
            return fastmalloc(size);
        else
            return block;
    } else if (size < LARGE_SIZE) {
        void *block = find_free_block(size, LARGE);
        if (block == NULL)
            return fastmalloc(size);
        else
            return block;
    } else {
        void *block = find_free_block(size, HUGE);
    }

    return NULL;
}

void *fastmalloc(size_t size)
{
    if (size < MEDIUM_SIZE) {
        struct malloc_header head = {0};
        size_t to_alloc = align(size + sizeof(head), 8);
        head.usable_size = to_alloc - sizeof(head);

        void *ptr = arena_alloc(&medium_pool, to_alloc);
        void *off = ((uint8_t *) ptr) + sizeof(head);
        *(struct malloc_header*) ptr = head;
        return off;
    } else if (size < LARGE_SIZE) {
        struct malloc_header head = {0};
        size_t to_alloc = align(size + sizeof(head), 8);
        head.usable_size = to_alloc - sizeof(head);

        void *ptr = arena_alloc(&large_pool, to_alloc);
        void *off = ((uint8_t *) ptr) + sizeof(head);
        *(struct malloc_header*) ptr = head;
        return off;
    } else {
        void *block = find_free_block(size, HUGE);
    }

    return NULL;
}

size_t malloc_usable_size(void *ptr)
{
    if (!initialized)
        malloc_init();

    if (!is_pointer_valid(ptr)) {
        /* TODO: finish implementation of invalid pointer */
        return 0;
    }
    
    void *base = ((uint8_t*) ptr) - sizeof(struct malloc_header);
    struct malloc_header *header = base;
    if (header->usable_size < 0)
        return 0;

    return header->usable_size;
}

void free(void *ptr)
{
    if (!initialized)
        malloc_init();

    if (!is_pointer_valid(ptr)) {
        /* TODO: finish implementation of invalid pointer */
        return;
    }

    void *base = ((uint8_t*) ptr) - sizeof(struct malloc_header);
    struct malloc_header *header = base;
    if (header->usable_size < 0) {
        /* TODO: double free ! */
        fprintf(stderr, "free(): double free detected!\n");
        abort();
    }
    header->usable_size = -header->usable_size;
}

/* TODO: Finish init */
static void malloc_init()
{
    medium_pool = arena_init(MEDIUM_SIZE * 1024); // 4 MB
    large_pool = arena_init(LARGE_SIZE * 2048); // 16 MB

    initialized = true;
}

static bool is_pointer_valid(void *ptr)
{
    struct arena *arenas[2] = {&medium_pool, &large_pool};
    for (int i = 0; i < 2; i++) {
        struct arena *iter = arenas[i];
        for (; iter != NULL; iter = iter->next) {
            uint8_t *adj = iter->data;
            adj += iter->allocated;

            /* WARN: Header should be checked too */
            if (ptr >= iter->data && ptr < (void*) adj)
                return true;
        }
    }

    /* TODO: check through huge allocations */

    return false;
}

/* TODO: Find block */
static void *find_free_block(size_t size, enum pools pool)
{
    uint8_t *adj = NULL;
    struct arena *iter = NULL;

    switch (pool) {
    case MEDIUM:
        iter = &medium_pool;
    case LARGE: /* FALLTHROUGH as both medium and large pools are arenas */
        if (iter == NULL)
            iter = &large_pool;

        for (; iter != NULL; iter = iter->next) {
            adj = iter->data;
            for (int i = iter->count; i > 0; i--) {
                struct malloc_header *header = (void*) adj;
                adj += sizeof(struct malloc_header);
                ssize_t prev_size = -header->usable_size;

                if (prev_size > 0) {
                    if (size <= prev_size)
                        return adj;
                    adj += prev_size;
                    continue;
                }
                adj += header->usable_size;
            }
        }
        break;
    case HUGE:
        break;
    }
    return NULL;
}
