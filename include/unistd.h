#include <inttypes.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#ifndef _UNISTD_H_
#define _UNISTD_H_

__BEGIN_DECLS

#include <stddef.h>

int brk(void *addr);
void *sbrk(intptr_t inc);

/* access() flags */
#define R_OK		4
#define W_OK		2
#define X_OK		1
#define F_OK		0

int access(char *name, int type);
int unlink(char *path);

extern char **environ;

int isatty(int fd);
int close(int fd);
long write(int fd, const void *buf, long n);
long read(int fd, void *buf, long n);

/* lseek() whence */
#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

long lseek(int fd, long offset, int whence);

int pipe(int fds[2]);
int dup(int fd);
int dup2(int fd, int fd2);

int fork(void);
int getpid(void);
int getppid(void);
gid_t getgid(void);
uid_t getuid(void);

int execve(char *path, char *argv[], char *envp[]);
int execle(char *path, ...);
int execvp(char *file, char *argv[]);
int execv(char *path, char *argv[]);

void _exit(int status);

int sleep(int n);

int pledge(const char *, const char *);

/* standard file descriptors */
#define STDIN_FILENO	0
#define STDOUT_FILENO	1
#define STDERR_FILENO	2

extern char *optarg;
extern int opterr;
extern int optind;
extern int optopt;
extern int optreset;

int getopt(int argc, char *const *argv, const char *optstring);

__END_DECLS

#endif
