#
# $FreeBSD: releng/11.0/sbin/mount_msdosfs/Makefile 298107 2016-04-16 07:45:30Z gjb $
#

PACKAGE=runtime
PROG=	mount_msdosfs
SRCS=	mount_msdosfs.c getmntopts.c
MAN=	mount_msdosfs.8
LIBADD=	kiconv

MOUNT=	${.CURDIR}/../mount
CFLAGS+= -I${MOUNT}

# Needs to be dynamically linked for optional dlopen() access to
# userland libiconv
NO_SHARED?=	NO

.PATH:	${MOUNT}

.include <bsd.prog.mk>
