# $FreeBSD: releng/11.0/sbin/geom/class/journal/Makefile 298107 2016-04-16 07:45:30Z gjb $

PACKAGE=runtime
.PATH: ${.CURDIR}/../../misc

GEOM_CLASS=	journal
SRCS+=	geom_journal_ufs.c

LIBADD=	ufs md

CFLAGS+=-I${.CURDIR}/../../../../sys

.include <bsd.lib.mk>
