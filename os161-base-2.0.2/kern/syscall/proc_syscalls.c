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
	#if OPT_LAB4
	struct proc *proc = curproc;
	struct thread *cur = curthread;
	proc_remthread(cur); //before to signal the and of the process remove the thread from the process
	spinlock_acquire(&proc->p_lock); //not needed in this case
	proc->exitStatus = status;
	spinlock_release(&proc->p_lock);
	V(proc->proc_semaphore);
	#else
	struct addrspace *ad;
	//sys_write(1, "exit with sys__exit\n", 20);
	ad = proc_getas();
	as_destroy(ad);
	(void) status;
	#endif
	thread_exit();
	
	panic("thread exit returned, should not happen");
	return;
}


#if OPT_LAB4
int sys_waitpid(int pid, int *status) {
	//KASSERT(process_table.vect[pid] != NULL);
	struct proc *p = getProc(pid);
	KASSERT(p != NULL);
	int pidRet = pid;
	int stat = proc_wait(p);
	*status = stat;
	return pidRet;
}

int sys_getpid(struct proc *proc) {
	return proc->pid;
}
#endif

