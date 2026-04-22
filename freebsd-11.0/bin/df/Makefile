#	@(#)Makefile	8.3 (Berkeley) 5/8/95
# $FreeBSD: releng/11.0/bin/df/Makefile 298107 2016-04-16 07:45:30Z gjb $

PACKAGE=runtime
MOUNT=	${.CURDIR}/../../sbin/mount
.PATH: ${MOUNT}

PROG=	df
SRCS=	df.c vfslist.c

CFLAGS+= -I${MOUNT}

LIBADD=	xo util

.include <bsd.prog.mk>
