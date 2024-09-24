#include <sys/cdefs.h>

#ifndef _DIRENT_H_
#define _DIRENT_H_

__BEGIN_DECLS

typedef struct __dirent_dir DIR;

struct dirent {
	unsigned long d_ino;
	unsigned long d_off;
	unsigned short d_reclen;
	char d_name[256];
};

DIR *opendir(char *path);
int closedir(DIR *dir);
struct dirent *readdir(DIR *dir);

__END_DECLS

#endif
