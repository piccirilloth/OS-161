#ifndef _FILE_SYSCALLS_H_
#define _FILE_SYSCALLS_H_
#include "opt-file_syscalls.h"
#if OPT_FILE_SYSCALLS

size_t sys_write(int fd, const char *buf, size_t count);

size_t sys_read(int fd, char *buf, size_t count);

#endif
#endif
