# $FreeBSD: releng/11.0/usr.bin/chkey/Makefile 275042 2014-11-25 14:29:10Z bapt $

.include <src.opts.mk>

.PATH: ${.CURDIR}/../newkey

PROG=	chkey
SRCS=	chkey.c generic.c update.c
CFLAGS+= -I${.CURDIR}/../newkey
.if ${MK_NIS} != "no"
CFLAGS+= -DYP
.endif
LIBADD+=	rpcsvc mp

.include <bsd.prog.mk>
