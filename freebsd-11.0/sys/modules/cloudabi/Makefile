# $FreeBSD: releng/11.0/sys/modules/cloudabi/Makefile 285786 2015-07-22 07:32:49Z ed $

.PATH: ${.CURDIR}/../../compat/cloudabi

KMOD=	cloudabi
SRCS=	cloudabi_clock.c cloudabi_errno.c cloudabi_fd.c cloudabi_file.c \
	cloudabi_futex.c cloudabi_mem.c cloudabi_proc.c cloudabi_random.c \
	cloudabi_sock.c cloudabi_thread.c vnode_if.h

.include <bsd.kmod.mk>
