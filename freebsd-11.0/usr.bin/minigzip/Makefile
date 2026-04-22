# $FreeBSD: releng/11.0/usr.bin/minigzip/Makefile 275042 2014-11-25 14:29:10Z bapt $

SRCDIR=	${.CURDIR}/../../lib/libz/test
.PATH:	${SRCDIR}

PROG=	minigzip

WARNS?=	5
CFLAGS+=-DUSE_MMAP
LIBADD=	z

.include <bsd.prog.mk>
