# $FreeBSD: releng/11.0/sbin/ggate/ggated/Makefile 294973 2016-01-28 03:24:06Z ngie $

.PATH: ${.CURDIR}/../shared

PROG=	ggated
MAN=	ggated.8
SRCS=	ggated.c ggate.c

LIBADD=	pthread util

CFLAGS+= -I${.CURDIR}/../shared

.include <bsd.prog.mk>
