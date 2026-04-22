# $FreeBSD: releng/11.0/usr.sbin/iovctl/Makefile 279461 2015-03-01 00:52:41Z rstone $

PROG=	iovctl
SRCS=	iovctl.c parse.c validate.c
LIBADD=	nv ucl m

CFLAGS+=-I${.CURDIR}/../../contrib/libucl/include

WARNS?=6

MAN=	\
	iovctl.8 \
	iovctl.conf.5 \

.include <bsd.own.mk>
.include <bsd.prog.mk>

