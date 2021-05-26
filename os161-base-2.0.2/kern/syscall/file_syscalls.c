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
#include "file_syscalls.h"

size_t sys_write(int fd, const char *buf, size_t count)
{
	unsigned int i, n=0;

	if(fd != 1)
	{
		kprintf("impossible to print on other destination\n");
		return -1;
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

	if(fd != 1)
	{
		kprintf("impossible to read from another source\n");
		return -1;
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
