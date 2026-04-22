# $FreeBSD: releng/11.0/sbin/ldconfig/Makefile 298107 2016-04-16 07:45:30Z gjb $

PACKAGE=runtime
PROG=	ldconfig
SRCS=	elfhints.c ldconfig.c shlib.c support.c
LDDIR?= ${.CURDIR}/../../libexec/rtld-aout
CFLAGS+=-I${LDDIR} -DFREEBSD_AOUT
MAN=	ldconfig.8

.PATH: ${LDDIR}

.include <bsd.prog.mk>
