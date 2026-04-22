#	From: @(#)Makefile	8.1 (Berkeley) 6/6/93
# $FreeBSD: releng/11.0/usr.sbin/fmtree/Makefile 277273 2015-01-16 21:39:08Z will $

.include <bsd.own.mk>

.PATH: ${.CURDIR}/../../usr.bin/cksum

PROG=	fmtree
MAN=	fmtree.8
SRCS=	compare.c crc.c create.c excludes.c misc.c mtree.c spec.c verify.c
SRCS+=	specspec.c

CFLAGS+= -DMD5 -DSHA1 -DRMD160 -DSHA256
LIBADD=	md

CLEANFILES+=	fmtree.8

fmtree.8: mtree.8
	${CP} ${.ALLSRC} ${.TARGET}

.include <bsd.prog.mk>
