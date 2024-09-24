#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pledge.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>

static int __PLEDGE_FLAGS = 0xFFFFFFFF;
static int ready = 0;

typedef struct {
    const char *group;
    const char *group_name;
} pledge_group;

static const pledge_group groups[] = {
    {PLEDGE_STDIO, "stdio"},
    {PLEDGE_RPATH, "rpath"},
    {PLEDGE_WPATH, "wpath"},
    {PLEDGE_CPATH, "cpath"},
    {PLEDGE_DPATH, "dpath"},
    {PLEDGE_FATTR, "fattr"},
    {PLEDGE_PROC, "proc"},
    {PLEDGE_EXEC, "exec"},
    {PLEDGE_SETTIME, "settime"},
    {PLEDGE_ID, "id"},
    {0}
};

static const char *_find_group(int mode);

/* XXX: execpromises must be implemented in the future */
int pledge(const char *promises, const char *execpromises)
{
    if (!ready) {
        __PLEDGE_FLAGS = 0;
        ready = 1;
    } else {
        errno = EPERM;
        return -1;
    }

    char *iter = (char*) promises;
    while (*iter) {
        switch (*iter) {
        case ' ':
            iter++;
            break;
        case 's':
            if (strncmp(iter, "stdio", 5) == 0) {
                __PLEDGE_FLAGS |= PLEDGE_STDIO;
                iter += 5;
                break;
            }
            if (strncmp(iter, "settime", 7) == 0) {
                __PLEDGE_FLAGS |= PLEDGE_SETTIME;
                iter += 7;
                break;
            }
        case 'r':
            if (strncmp(iter, "rpath", 5) == 0) {
                __PLEDGE_FLAGS |= PLEDGE_RPATH;
                iter += 5;
                break;
            }
        case 'w':
            if (strncmp(iter, "wpath", 5) == 0) {
                __PLEDGE_FLAGS |= PLEDGE_WPATH;
                iter += 5;
                break;
            }
        case 'c':
            if (strncmp(iter, "cpath", 5) == 0) {
                __PLEDGE_FLAGS |= PLEDGE_CPATH;
                iter += 5;
                break;
            }
        case 'd':
            if (strncmp(iter, "dpath", 5) == 0) {
                __PLEDGE_FLAGS |= PLEDGE_DPATH;
                iter += 5;
                break;
            }
        case 'f':
            if (strncmp(iter, "fattr", 5) == 0) {
                __PLEDGE_FLAGS |= PLEDGE_FATTR;
                iter += 5;
                break;
            }
        case 'p':
            if (strncmp(iter, "proc", 4) == 0) {
                __PLEDGE_FLAGS |= PLEDGE_PROC;
                iter += 4;
                break;
            }
        case 'e':
            if (strncmp(iter, "exec", 4) == 0) {
                __PLEDGE_FLAGS |= PLEDGE_EXEC;
                iter += 4;
                break;
            }
        case 'i':
            if (strncmp(iter, "id", 2) == 0) {
                __PLEDGE_FLAGS |= PLEDGE_ID;
                iter += 2;
                break;
            }
        default:
            errno = EINVAL;
            return -1;
        }
    }

    return 0;
}

void __pledge_check(const int mode, int syscall_num)
{
    if (!(__PLEDGE_FLAGS & mode)) {
        __PLEDGE_FLAGS = 0xFFFFFFFF; /* program is going to end anyway */
        fprintf(stderr, "*** pledge \"%s\" violated ***: syscall %d\n",
            _find_group(mode), syscall_num);
        signal(SIGABRT, SIG_DFL);
        abort();
    }
}

static const char *_find_group(int mode)
{
    const pledge_group *p_iter = groups;

    while (p_iter->group_name) {
        if (p_iter->group == mode)
            return p_iter->group_name;
        p_iter++;
    }

    return NULL;
}
