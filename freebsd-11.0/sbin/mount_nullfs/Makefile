#	@(#)Makefile	8.3 (Berkeley) 3/27/94
# $FreeBSD: releng/11.0/sbin/mount_nullfs/Makefile 298107 2016-04-16 07:45:30Z gjb $

PACKAGE=runtime
PROG=	mount_nullfs
SRCS=	mount_nullfs.c getmntopts.c
MAN=	mount_nullfs.8

MOUNT=	${.CURDIR}/../mount
CFLAGS+=-I${MOUNT}

.PATH:	${MOUNT}

.include <bsd.prog.mk>
