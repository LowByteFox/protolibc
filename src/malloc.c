#include <stddef.h>
#include <stdbool.h>
#include <arena.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define PGSIZE 4096
#define align(x, align) (((x) + (align) - 1) & ~((align) - 1))

#define MEDIUM_SIZE 4096
#define LARGE_SIZE  8192

struct malloc_header {
    ssize_t usable_size;
};

struct huge_alloc_list {
    struct malloc_header *prev;
    struct malloc_header *next;
};

struct alloc_pool {
    struct arena arena;
    struct arena *tail;
};

struct huge_alloc_pool {
    struct malloc_header *first;
    struct malloc_header *tail;
};

enum pools {
    MEDIUM,
    LARGE,
    HUGE
};

/* context vars */
static bool initialized = false;
static struct arena *found_arena = NULL;

/* alloc pools */
static struct alloc_pool medium_pool;
static struct alloc_pool large_pool;
static struct huge_alloc_pool huge_pool;

/* functions */
static void malloc_init();
static void update_tail(struct alloc_pool *pool);
static bool is_pointer_valid(void *ptr);
static void *find_free_block(size_t size, enum pools pool);

/* huge allocations helper functions */
static void *huge_alloc(size_t size);
static struct huge_alloc_list *get_list(struct malloc_header *header);
static void huge_alloc_rm(struct malloc_header *header);

void *malloc(size_t size)
{
    if (!initialized)
        malloc_init();

    ssize_t size_check = size;
    if (size_check < 0) {
        fprintf(stderr, "malloc(): invalid size\n");
        abort();
    }

    if (size == 0)
        return NULL;

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
        return fastmalloc(size);
    }

    return NULL;
}

void *fastmalloc(size_t size)
{
    if (size < MEDIUM_SIZE) {
        struct malloc_header head = {0};
        size_t to_alloc = align(size + sizeof(head), 8);
        head.usable_size = to_alloc - sizeof(head);

        void *ptr = arena_alloc(medium_pool.tail, to_alloc);
        void *off = ((uint8_t *) ptr) + sizeof(head);
        *(struct malloc_header*) ptr = head;

        update_tail(&medium_pool);
        return off;
    } else if (size < LARGE_SIZE) {
        struct malloc_header head = {0};
        size_t to_alloc = align(size + sizeof(head), 8);
        head.usable_size = to_alloc - sizeof(head);

        void *ptr = arena_alloc(large_pool.tail, to_alloc);
        void *off = ((uint8_t *) ptr) + sizeof(head);
        *(struct malloc_header*) ptr = head;

        update_tail(&large_pool);
        return off;
    } else {
        return huge_alloc(size);
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

    if (ptr == NULL)
        return;

    if (!is_pointer_valid(ptr)) {
        /* TODO: finish implementation of invalid pointer */
        fprintf(stderr, "free(): invalid pointer!\n");
        abort();
    }

    fastfree(ptr);
}

void fastfree(void *ptr) {
    void *base = ((uint8_t*) ptr) - sizeof(struct malloc_header);
    struct malloc_header *header = base;
    if (header->usable_size < 0) {
        /* TODO: double free ! */
        fprintf(stderr, "free(): double free detected!\n");
        abort();
    }
    if (header->usable_size >= LARGE_SIZE) {
        huge_alloc_rm(header);
        munmap(header, header->usable_size + sizeof(*header) +
            sizeof(struct huge_alloc_list));
    } else
        header->usable_size = -header->usable_size;
}

void *realloc(void *ptr, size_t new_size)
{
    if (!initialized)
        malloc_init();

    ssize_t size_check = new_size;
    if (size_check < 0) {
        fprintf(stderr, "realloc(): invalid size\n");
        abort();
    }

    if (new_size == 0)
        return NULL;

    if (ptr == NULL)
        return malloc(new_size);

    if (!is_pointer_valid(ptr)) {
        /* TODO: finish implementation of invalid pointer */
        fprintf(stderr, "realloc(): invalid pointer!\n");
        abort();
    }

    void *base = ((uint8_t*) ptr) - sizeof(struct malloc_header);
    struct malloc_header *header = base;

    if (header->usable_size < MEDIUM_SIZE || header->usable_size < LARGE_SIZE) {
        size_t old_alloc_size = header->usable_size + sizeof(*header);

        uint8_t *adj = found_arena->data;
        adj += found_arena->pos - old_alloc_size;

        /* last allocation detected */
        if (adj == (void*) header) {
            size_t to_alloc = align(new_size + sizeof(*header), 8);
            size_t space_left = found_arena->allocated - found_arena->pos;

            if (to_alloc - old_alloc_size <= space_left) {
                /* can expand */
                found_arena->pos += to_alloc - old_alloc_size;
                return ptr;
            } else {
                /* cannot expand */
                void *new = fastmalloc(new_size);
                memcpy(new, ptr, header->usable_size);
                fastfree(ptr);

                return new;
            }
        } else {
            void *new = fastmalloc(new_size);
            memcpy(new, ptr, header->usable_size);
            fastfree(ptr);

            return new;
        }
    } else {
        void *new = fastmalloc(new_size);
        memcpy(new, ptr, header->usable_size);
        fastfree(ptr);

        return new;
    }

    return NULL;
}

void *fastrealloc(void *ptr, size_t new_size)
{
    ssize_t size_check = new_size;
    if (size_check < 0) {
        fprintf(stderr, "realloc(): invalid size\n");
        abort();
    }

    if (new_size == 0)
        return NULL;

    void *base = ((uint8_t*) ptr) - sizeof(struct malloc_header);
    struct malloc_header *header = base;

    void *new = fastmalloc(new_size);
    memcpy(new, ptr, header->usable_size);
    fastfree(ptr);

    return new;
}

void *calloc(size_t nmemb, size_t size)
{
    size_t total = nmemb * size;
    void *ptr = malloc(total);

    if (ptr == NULL)
        return NULL;

    memset(ptr, 0, total);
    return ptr;
}

void *fastcalloc(size_t nmemb, size_t size)
{
    size_t total = nmemb * size;
    void *ptr = fastmalloc(total);

    if (ptr == NULL)
        return NULL;

    memset(ptr, 0, total);
    return ptr;
}

void *reallocarray(void *ptr, size_t nmemb, size_t size)
{
    return realloc(ptr, nmemb * size);
}

void *recallocarray(void *ptr, size_t oldnmemb, size_t nmemb, size_t size)
{
    if (ptr == NULL)
        return calloc(nmemb, size);

    size_t total = nmemb * size;
    void *new = realloc(ptr, total);

    if (new == NULL)
        return NULL;

    uint8_t *adj = new;
    new += oldnmemb * size;

    memset(adj, 0, total - oldnmemb * size);

    return adj;
}

void freezero(void *ptr, size_t size)
{
    if (!is_pointer_valid(ptr)) {
        fprintf(stderr, "freezero(): invalid pointer!\n");
        abort();
    }

    memset(ptr, 0, size);
    free(ptr);
}

/* TODO: MALLOC_OPTIONS */
static void malloc_init()
{
    medium_pool.arena = arena_init(MEDIUM_SIZE * 1024); // 4 MB
    medium_pool.tail = &medium_pool.arena;
    large_pool.arena = arena_init(LARGE_SIZE * 2048); // 16 MB
    large_pool.tail = &large_pool.arena;

    huge_pool.first = 0;
    huge_pool.tail = 0;

    initialized = true;
}

static bool is_pointer_valid(void *ptr)
{
    found_arena = NULL;
    struct arena *arenas[2] = {&medium_pool.arena, &large_pool.arena};
    for (int i = 0; i < 2; i++) {
        struct arena *iter = arenas[i];
        for (; iter != NULL; iter = iter->next) {
            uint8_t *adj = iter->data;
            adj += iter->allocated;

            if (ptr >= (void*) iter && ptr < (void*) adj) {
                found_arena = iter;
                return true;
            }
        }
    }

    struct malloc_header *iter = huge_pool.first;
    while (iter != NULL) {
        uint8_t *adj = (void*) iter;
        adj += iter->usable_size;

        if (ptr >= (void*) iter && ptr < (void*) adj)
            return true;

        iter = get_list(iter)->next;
    }

    return false;
}

static void *find_free_block(size_t size, enum pools pool)
{
    uint8_t *adj = NULL;
    struct arena *iter = NULL;

    switch (pool) {
    case MEDIUM:
        iter = &medium_pool.arena;
    case LARGE: /* FALLTHROUGH as both medium and large pools are arenas */
        if (iter == NULL)
            iter = &large_pool.arena;

        for (; iter != NULL; iter = iter->next) {
            adj = iter->data;
            for (int i = iter->count; i > 0; i--) {
                struct malloc_header *header = (void*) adj;
                adj += sizeof(struct malloc_header);
                ssize_t prev_size = -header->usable_size;

                if (prev_size > 0) {
                    if (size <= prev_size) {
                        /* flag the block as used */
                        header->usable_size = -header->usable_size;
                        return adj;
                    }
                    adj += prev_size;
                    continue;
                }
                adj += header->usable_size;
            }
        }
        break;
    case HUGE: /* There won't be free blocks */
        break;
    }
    return NULL;
}

static void update_tail(struct alloc_pool *pool)
{
    while (pool->tail->next != NULL)
        pool->tail = pool->tail->next;
}

static void *huge_alloc(size_t size)
{
    struct malloc_header head = {0};
    size_t to_alloc = align(size + sizeof(head) +
        sizeof(struct huge_alloc_list), PGSIZE);

    head.usable_size = to_alloc - sizeof(head) -
        sizeof(struct huge_alloc_list);

    void *ptr = mmap(NULL, to_alloc, PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    *(struct malloc_header*) ptr = head;
    uint8_t *adj = ptr;
    adj += sizeof(head);
    adj += head.usable_size;
    memset(adj, 0, sizeof(struct huge_alloc_list));

    if (huge_pool.first == NULL) {
        huge_pool.first = ptr;
        huge_pool.tail = ptr;
    } else {
        struct huge_alloc_list *list = get_list(ptr);
        struct huge_alloc_list *prev = get_list(huge_pool.tail);

        list->prev = huge_pool.tail;
        prev->next = ptr;
        huge_pool.tail = ptr;
    }

    return ((uint8_t*) ptr) + sizeof(head);
}

static struct huge_alloc_list *get_list(struct malloc_header *header)
{
    uint8_t *adj = (void*) header;
    adj += sizeof(*header);
    adj += header->usable_size;
    return (struct huge_alloc_list*) adj;
}

static void huge_alloc_rm(struct malloc_header *header)
{
    struct huge_alloc_list *list = get_list(header);

    /* last one in list */
    if (list->prev == NULL && list->next == NULL) {
        huge_pool.first = 0;
        huge_pool.tail = 0;
        return;
    }

    /* pop front */
    if (list->prev == NULL) {
        struct huge_alloc_list *next = get_list(list->next);
        huge_pool.first = list->next;
        next->prev = NULL;
        return;
    }

    /* pop back */
    if (list->next == NULL) {
        struct huge_alloc_list *prev = get_list(list->prev);
        prev->next = NULL;
        huge_pool.tail = list->prev;
        return;
    }

    struct huge_alloc_list *next = get_list(list->next);
    struct huge_alloc_list *prev = get_list(list->prev);

    prev->next = list->next;
    next->prev = list->prev;
}
