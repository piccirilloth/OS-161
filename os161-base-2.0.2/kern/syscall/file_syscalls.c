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
#include <limits.h>
#include <uio.h>
#include <copyinout.h>
#include <vnode.h>
#include "file_syscalls.h"
#include "opt-lab5.h"

size_t sys_write(int fd, const char *buf, size_t count)
{
	unsigned int i, n=0;

	if(fd > 2)
	{
		struct proc *p = curproc;
		struct iovec vio;
		struct uio ku;
		int result;
		int nwrite;
		off_t offset = p->openFileTable[fd].offset;
		char *kbuf = kmalloc(count * sizeof(char));
		copyin((userptr_t) buf, kbuf, count);
		uio_kinit(&vio, &ku, kbuf, count, offset, UIO_WRITE);
		result = VOP_WRITE(p->openFileTable[fd].v, &ku);
		if(result)
			return result;
		p->openFileTable[fd].offset = ku.uio_offset;
		nwrite = count - ku.uio_resid;
		kfree(kbuf);
		return nwrite;
	}

	for(i=0; i<count; i++)
	{
		if(buf[i] != -1)
		{
			putch(buf[i]);
			n++;
		}
		else
			break;
	}
	return n;
}

size_t sys_read(int fd, char *buf, size_t count)
{
	unsigned int i, n=0;
	//char c;

	if(fd > 2)
	{
		struct proc *p = curproc;
		struct iovec vio;
		struct uio ku;
		int nread;
		int result;
		off_t offset = p->openFileTable[fd].offset;
		char *kbuf = kmalloc(count * sizeof(char));
		
		uio_kinit(&vio, &ku, kbuf, count, offset, UIO_READ);
		result = VOP_READ(p->openFileTable[fd].v, &ku);
		if(result)
			return result;
		p->openFileTable[fd].offset = ku.uio_offset;
		nread = count - ku.uio_resid;
		copyout(kbuf, (userptr_t) buf, nread);
		kfree(kbuf);
		return nread;
	}

	for(i=0; i<count; i++)
	{
		buf[i] = getch();
		//if(c == 27)
		//	break;
		if(buf[i] < 0)
			return n;
		else
			n++;
	}
	return n;
}

#if OPT_LAB5
int sys_open(userptr_t fileName, int flags, mode_t mode, int *errp) {
	int fd;
	int result;
	int i;
	struct proc *p = curproc;
	for(i=0; i<10*OPEN_MAX; i++) 
		if(systemFileTable[i].v == NULL) {
			systemFileTable[i].offset = 0;
			systemFileTable[i].refCount = 1;
			break;
		}
	result = vfs_open((char *) fileName, flags, mode, &(systemFileTable[i].v));
	if(result) {
		*errp = ENOENT;
		return -1;
	}
		
	for(fd=3; fd<OPEN_MAX; fd++) {
		if(p->openFileTable[fd].v == NULL) {
			p->openFileTable[fd].v = systemFileTable[i].v;
			p->openFileTable[fd].offset = systemFileTable[i].offset;
			p->openFileTable[fd].refCount = systemFileTable[i].refCount;
			break;
		}
	}
	*errp = 0;
	return fd;
}

int sys_close(int fd) {
	struct proc *p = curproc;
	int i;
	if(fd > OPEN_MAX || fd < 0 || p->openFileTable[fd].v == NULL)
		return -1;
	for(i=0; i<10*OPEN_MAX; i++)
		if(p->openFileTable[fd].v == systemFileTable[i].v) {
			if(systemFileTable[i].refCount > 1)
				systemFileTable[i].refCount--;
			else {
				systemFileTable[i].v = NULL;
			}
		}
	vfs_close(p->openFileTable[fd].v);
	p->openFileTable[fd].v = NULL;
	return 0;
}

#endif
