# $FreeBSD: releng/11.0/usr.bin/pamtest/Makefile 275042 2014-11-25 14:29:10Z bapt $

OPENPAM=	${.CURDIR}/../../contrib/openpam
.PATH: ${OPENPAM}/include ${OPENPAM}/bin/pamtest

PROG= pamtest
SRCS= pamtest.c

LIBADD=	pam

.include <bsd.prog.mk>
