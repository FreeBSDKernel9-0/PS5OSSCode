# $FreeBSD: releng/11.0/sbin/ggate/ggatec/Makefile 275030 2014-11-25 11:23:12Z bapt $

.PATH: ${.CURDIR}/../shared

PROG=	ggatec
MAN=	ggatec.8
SRCS=	ggatec.c ggate.c

CFLAGS+= -DMAX_SEND_SIZE=32768
CFLAGS+= -DLIBGEOM
CFLAGS+= -I${.CURDIR}/../shared

LIBADD=	geom util pthread

.include <bsd.prog.mk>
