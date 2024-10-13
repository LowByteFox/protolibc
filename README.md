# protolibc

Protolibc is a small C standard library based on neatlibc <br>
This libc was patched so you'll be able to compile it using tinycc <br>
and any other compiler

## Features
- BSD functions like `setprogname`, `getprogname`, `strlcpy` and `strlcat`
- OpenBSD-like `pledge`
- custom 2 assert functions `assert2` and `env_assert`
- `__PROTOGEN__` macro indicating you are linking against protolibc
- custom malloc implementation `protomalloc`

### protomalloc
Custom implementation of malloc<br>
set of functions:
- `malloc` - your standard malloc
- `calloc` - your standard calloc
- `realloc` - your standard realloc
- `free` - your standard free
- `fastmalloc` - fast version of the function malloc, doesn't search for freed blocks
- `fastcalloc` - same as fastmalloc
- `fastrealloc` - a little slower realloc but does a lot less operations
- `reallocarray` - safe array reallocations
- `recallocarray` - same as reallocarray but it zeroes out new data
- `freezero` - discards data on the pointer and frees
- `malloc_usable_size` - optain size of block of allocated memory

#### malloc options

set either through:
- env `MALLOC_OPTIONS`
- variable `const char *malloc_options` - defined in `stdlib.h`
- config file `/etc/malloc.conf`

set of options:
- C - "Canaries", add canaries at the end of allocations in order to detect heap overflows. The canary's content is checked when free is called. If it has been corrupted, the process is aborted.
- D - "Dump", malloc() will dump a leak report at exit. To view the leak report use protolibc memdump viewer. This option can be followed by + to also dump memory content
- F - "Freecheck", if invalid pointer is found, instead of abort(), errno value will be set
- J - "Junking", add junk value to newly allocated memory to indicate unused space
- V - "Verbose", used for debug purposes, prints information to stderr
- X - "Xmalloc", rather than return failure, abort() the program with a diagnostic message on stderr
