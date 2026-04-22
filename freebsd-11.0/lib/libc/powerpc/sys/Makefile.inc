# $FreeBSD: releng/11.0/lib/libc/powerpc/sys/Makefile.inc 302092 2016-06-22 21:11:27Z brooks $

MDASM+=	brk.S cerror.S exect.S ptrace.S sbrk.S setlogin.S

# Don't generate default code for these syscalls:
NOASM=	break.o exit.o getlogin.o openbsd_poll.o sstk.o yield.o

PSEUDO=	_getlogin.o _exit.o
