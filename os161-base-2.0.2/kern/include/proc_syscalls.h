#ifndef _PROC_SYSCALLS_H_
#define _PROC_SYSCALLS_H_
#include "opt-proc_syscalls.h"
#if OPT_PROC_SYSCALLS

void sys__exit(int status);
int sys_waitpid(int pid, int *status);
int sys_getpid(struct proc *proc);

#endif
#endif
