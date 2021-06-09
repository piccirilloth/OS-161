#ifndef _FILE_SYSCALLS_H_
#define _FILE_SYSCALLS_H_
#include <limits.h>
#include <vnode.h>
#include "opt-file_syscalls.h"
#include "opt-lab5.h"

#if OPT_LAB5
struct openfile {
	struct vnode *v;
	off_t offset;
	int refCount;
};

struct openfile systemFileTable[10*OPEN_MAX];

int sys_open(userptr_t fileName, int flags, mode_t mode, int *errp);
int sys_close(int fd);
#endif

#if OPT_FILE_SYSCALLS

size_t sys_write(int fd, const char *buf, size_t count);

size_t sys_read(int fd, char *buf, size_t count);

#endif

#endif
