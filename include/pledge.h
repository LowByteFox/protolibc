#ifndef _PLEDGE_H_
#define _PLEDGE_H_

#define PLEDGE_STDIO    1
#define PLEDGE_RPATH    1 << 1
#define PLEDGE_WPATH    1 << 2
#define PLEDGE_CPATH    1 << 3
#define PLEDGE_DPATH    1 << 4
#define PLEDGE_FATTR    1 << 5
#define PLEDGE_PROC     1 << 6
#define PLEDGE_EXEC     1 << 7
#define PLEDGE_SETTIME  1 << 8
#define PLEDGE_ID       1 << 9

void __pledge_check(const int mode, int syscall_num);

#endif
