# $FreeBSD: releng/11.0/sbin/ggate/ggatel/Makefile 275030 2014-11-25 11:23:12Z bapt $

.PATH: ${.CURDIR}/../shared

PROG=	ggatel
MAN=	ggatel.8
SRCS=	ggatel.c ggate.c

CFLAGS+= -DLIBGEOM
CFLAGS+= -I${.CURDIR}/../shared

LIBADD=	geom util

.include <bsd.prog.mk>
