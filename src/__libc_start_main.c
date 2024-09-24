#include <string.h>
int main(int, char**, char**);

const char *__progname = 0;

int __libc_start_main(int argc, char **argv, char **env) {
    __progname = strrchr(*argv, '/') + 1;
    return main(argc, argv, env);
}
