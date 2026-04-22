# $FreeBSD: releng/11.0/usr.bin/atm/sscop/Makefile 275042 2014-11-25 14:29:10Z bapt $

CONTRIB= ${.CURDIR}/../../../contrib/ngatm/sscop

.PATH:	${CONTRIB}

PROG=	sscop
SRCS=	common.c sscop_main.c
CFLAGS+= -I${CONTRIB} -DUSE_LIBBEGEMOT

LIBADD=	begemot ngatm

.include <bsd.prog.mk>
