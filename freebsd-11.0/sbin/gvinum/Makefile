# $FreeBSD: releng/11.0/sbin/gvinum/Makefile 298107 2016-04-16 07:45:30Z gjb $

PACKAGE=runtime
PROG=	gvinum
SRCS=	gvinum.c gvinum.h geom_vinum_share.c
MAN=	gvinum.8

WARNS?=	2
CFLAGS+=	-I${.CURDIR}/../../sys -I${DESTDIR}/${INCLUDEDIR}/edit

LIBADD=	edit geom

.PATH: ${.CURDIR}/../../sys/geom/vinum

.include <bsd.prog.mk>
