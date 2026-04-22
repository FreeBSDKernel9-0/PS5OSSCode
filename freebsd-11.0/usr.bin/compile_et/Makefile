# $FreeBSD: releng/11.0/usr.bin/compile_et/Makefile 275042 2014-11-25 14:29:10Z bapt $

.PATH: ${.CURDIR}/../../contrib/com_err

PROG=	compile_et
SRCS=	compile_et.c parse.y lex.l
LIBADD=	roken vers
CFLAGS+=-I. -I${.CURDIR}/../../contrib/com_err

WARNS?=	0

.include <bsd.prog.mk>
