#include <types.h>
#include <kern/errno.h>
#include <kern/unistd.h>
#include <kern/fcntl.h>
#include <lib.h>
#include <proc.h>
#include <current.h>
#include <addrspace.h>
#include <vm.h>
#include <vfs.h>
#include <syscall.h>
#include <test.h>
#include <proc.h>
#include <thread.h>
#include "proc_syscalls.h"
#include "file_syscalls.h"
#include <synch.h>

void sys__exit(int status)
{
	struct addrspace *ad;

	#if OPT_LAB4
	struct proc *proc = curproc;
	spinlock_acquire(&proc->p_lock);
	curproc->exitStatus = status;
	V(proc->proc_semaphore);
	spinlock_release(&proc->p_lock);
	#endif
	//sys_write(1, "exit with sys__exit\n", 20);
	ad = proc_getas();
	as_destroy(ad);
	thread_exit();
	panic("thread exit returned, should not happen");
	(void) status;
	return;
}
