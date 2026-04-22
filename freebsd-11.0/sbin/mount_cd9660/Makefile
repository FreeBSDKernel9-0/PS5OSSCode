#	@(#)Makefile	8.3 (Berkeley) 3/27/94
# $FreeBSD: releng/11.0/sbin/mount_cd9660/Makefile 298107 2016-04-16 07:45:30Z gjb $

PACKAGE=runtime
PROG=	mount_cd9660
SRCS=	mount_cd9660.c getmntopts.c
MAN=	mount_cd9660.8
LIBADD=	kiconv

MOUNT=	${.CURDIR}/../mount
CFLAGS+= -I${MOUNT}

# Needs to be dynamically linked for optional dlopen() access to
# userland libiconv
NO_SHARED?=	NO

.PATH:	${MOUNT}

.include <bsd.prog.mk>
