#include "include/pledge.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __PROTOGEN__
# include <unistd.h>
#endif

#define _(expr) #expr

static const char *registers[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};

typedef struct {
    const char *name;
    int opcode, nargs, pledge;
} syscall;

typedef struct {
    const char *group;
    const char *group_name;
} pledge_table;

static const pledge_table groups[] = {
    /* pledge group,     name */
    {_(PLEDGE_STDIO),      "stdio"},
    {_(PLEDGE_RPATH),      "rpath"},
    {_(PLEDGE_WPATH),      "wpath"},
    {_(PLEDGE_CPATH),      "cpath"},
    {_(PLEDGE_DPATH),      "dpath"},
    {_(PLEDGE_FATTR),      "fattr"},
    {_(PLEDGE_PROC),       "proc"},
    {_(PLEDGE_EXEC),       "exec"},
    {_(PLEDGE_SETTIME),    "settime"},
    {_(PLEDGE_ID),         "id"},
    {0}
};

static const syscall syscalls[] = {
    /* C fn,        opcode, nargs, pledge group */
    {"read",            0,      3, PLEDGE_STDIO},
    {"write",           1,      3, PLEDGE_STDIO},
    {"open",            2,      3, PLEDGE_STDIO},
    {"close",           3,      1, PLEDGE_STDIO},
    {"fork",            57,     0, PLEDGE_PROC},
    {"waitpid",         61,     4, PLEDGE_STDIO},
    {"creat",           85,     2, PLEDGE_STDIO},
    {"link",            86,     2, PLEDGE_CPATH},
    {"unlink",          87,     1, PLEDGE_CPATH},
    {"execve",          59,     3, PLEDGE_EXEC},
    {"chdir",           80,     1, PLEDGE_RPATH},
    {"time",            201,    1, PLEDGE_STDIO}, /* INFO: Not sure */
    {"mknod",           133,    3, PLEDGE_DPATH},
    {"chmod",           90,     2, PLEDGE_RPATH},
    {"lseek",           8,      3, PLEDGE_STDIO},
    {"getpid",          39,     0, PLEDGE_STDIO},
    {"mount",           165,    5, PLEDGE_STDIO},
    {"umount2",         166,    2, PLEDGE_STDIO},
    {"setuid",          105,    1, PLEDGE_ID},
    {"getuid",          102,    0, PLEDGE_STDIO}, /* INFO: OpenBSD pledge says so */
    {"utime",           132,    2, PLEDGE_FATTR},
    {"utimes",          235,    2, PLEDGE_FATTR},
    {"access",          21,     2, PLEDGE_RPATH},
    {"sync",            162,    0, PLEDGE_STDIO},
    {"syncfs",          306,    1, PLEDGE_STDIO},
    {"kill",            62,     2, PLEDGE_PROC},
    {"mkdir",           83,     2, PLEDGE_CPATH},
    {"rmdir",           84,     1, PLEDGE_CPATH},
    {"dup",             32,     1, PLEDGE_STDIO},
    {"dup2",            33,     2, PLEDGE_STDIO},
    {"dup3",            292,    3, PLEDGE_STDIO},
    {"pipe",            22,     1, PLEDGE_STDIO},
    {"pipe2",           293,    2, PLEDGE_STDIO},
    {"setgid",          106,    1, PLEDGE_ID},
    {"getgid",          104,    0, PLEDGE_STDIO}, /* INFO: OpenBSD pledge says so */
    {"geteuid",         107,    0, PLEDGE_STDIO}, /* same here */
    {"getegid",         108,    0, PLEDGE_STDIO}, /* same here */
    {"getppid",         110,    0, PLEDGE_STDIO}, /* same here */
    {"ioctl",           16,     3, PLEDGE_STDIO}, /* INFO: Not sure */
    {"fcntl",           72,     3, PLEDGE_STDIO},
    {"setsid",          124,    0, PLEDGE_PROC},
    {"gettimeofday",    96,     2, PLEDGE_STDIO},
    {"settimeofday",    164,    2, PLEDGE_SETTIME},
    {"mmap",            9,      6, PLEDGE_STDIO}, /* INFO: OpenBSD pledge says so */
    {"mprotect",        10,     3, PLEDGE_STDIO}, /* same here */
    {"munmap",          11,     2, PLEDGE_STDIO}, /* same here */
    {"stat",            4,      2, PLEDGE_RPATH},
    {"fstat",           5,      2, PLEDGE_RPATH},
    {"lstat",           6,      2, PLEDGE_RPATH},
    {"clone",           56,     5, PLEDGE_PROC},
    {"uname",           63,     1, PLEDGE_STDIO},
    {"fchdir",          81,     1, PLEDGE_STDIO},
    {"rename",          82,     2, PLEDGE_CPATH},
    {"getdents",        78,     3, PLEDGE_RPATH},
    {"getdents64",      217,    3, PLEDGE_RPATH},
    {"nanosleep",       35,     2, PLEDGE_STDIO}, /* INFO: OpenBSD pledge says so */
    {"poll",            7,      3, PLEDGE_STDIO},
    {"chown",           92,     3, PLEDGE_RPATH},
    {"getcwd",          79,     2, PLEDGE_WPATH},
    {"sigaction",       13,     4, PLEDGE_STDIO}, /* INFO: OpenBSD pledge says so */
    {"sigprocmask",     14,     4, PLEDGE_STDIO}, /* same here */
    {"sigreturn",       15,     0, PLEDGE_STDIO}, /* same here */
    {"fsync",           74,     1, PLEDGE_STDIO},
    {"fdatasync",       75,     1, PLEDGE_STDIO},
    {"truncate",        76,     2, PLEDGE_STDIO},
    {"ftruncate",       77,     2, PLEDGE_STDIO},
    {0}
};

void syscall_header(FILE *f, const syscall *s) {
    fprintf(f, ".global %s\n"
               "%s:\n", s->name, s->name);
}

void store_registers(FILE *f, const syscall *s) {
    for (int i = s->nargs - 1; i >= 0; i--) {
        fprintf(f, "    push %s\n", registers[i]);
    }
}

void call_pledge_check(FILE *f, const syscall *s) {
    fprintf(f, "    mov $%d, %%eax\n"
               "    mov %%rax, %%rsi\n"
               "    mov $%d, %%eax\n"
               "    mov %%rax, %%rdi\n"
               "    call __pledge_check\n", s->opcode, s->pledge);
}

void restore_registers(FILE *f, const syscall *s) {
    for (int i = 0; i < s->nargs; i++) {
        fprintf(f, "    pop %s\n", registers[i]);
    }
}

void perform_syscall(FILE *f, const syscall *s) {
    fprintf(f, "    mov $%d, %%eax\n"
               "    jmp __syscall\n", s->opcode);
}

void syscall_footer(FILE *f) {
    fprintf(f, "\n");
}

void pledge_table_header(FILE *f) {
    fprintf(f, "static const pledge_group groups[] = {\n");
}

void pledge_table_entry(FILE *f, const pledge_table *e) {
    fprintf(f, "    {%s, \"%s\"},\n", e->group, e->group_name);
}

void pledge_table_footer(FILE *f) {
    fprintf(f, "    {0}\n"
               "};\n");
}

int main(int argc, char **argv) {
    if (argc != 2) {
        return 1;
    }

    if (strcmp(argv[1], "amd64") == 0) {
    } else {
        fprintf(stderr, "Unknown CPU arch: %s\n", argv[1]);
        return 1;
    }

#ifdef __PROTOGEN__
    if (pledge("stdio", NULL) == -1) {
        perror("pledge");
        return 1;
    }
#endif

    char path_buf[256] = {0};
    snprintf(path_buf, 255, "arch/%s/syscall.s", argv[1]);
    
    FILE *f = fopen(path_buf, "a");

    const syscall *iter = syscalls;

    while (iter->name) {
        syscall_header(f, iter);
        store_registers(f, iter);
        call_pledge_check(f, iter);
        restore_registers(f, iter);
        perform_syscall(f, iter);
        syscall_footer(f);
        iter++;
    }

    fclose(f);

    if (getenv("PLEDGE_TABLE") == NULL) {
        return 0;
    }

    const pledge_table *p_iter = groups;

    pledge_table_header(stdout);

    while (p_iter->group_name) {
        pledge_table_entry(stdout, p_iter);
        p_iter++;
    }

    pledge_table_footer(stdout);

    return 0;
}
