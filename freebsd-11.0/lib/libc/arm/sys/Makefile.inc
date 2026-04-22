# $FreeBSD: releng/11.0/lib/libc/arm/sys/Makefile.inc 302092 2016-06-22 21:11:27Z brooks $

SRCS+=	__vdso_gettc.c

MDASM= Ovfork.S brk.S cerror.S ptrace.S sbrk.S shmat.S sigreturn.S syscall.S

# Don't generate default code for these syscalls:
NOASM=	break.o exit.o getlogin.o openbsd_poll.o sstk.o vfork.o yield.o

PSEUDO= _exit.o _getlogin.o
