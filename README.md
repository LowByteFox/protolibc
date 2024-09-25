# protolibc

Protolibc is a small C standard library based on neatlibc <br>
This libc was patched so you'll be able to compile it using tinycc <br>
and any other compiler

## Features
- BSD functions like `setprogname`, `getprogname`, `strlcpy` and `strlcat`
- OpenBSD-like `pledge`
- custom 2 assert functions `assert2` and `env_assert`
- `__PROTOGEN__` macro indicating you are linking against protolibc
