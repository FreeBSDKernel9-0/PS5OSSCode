# $FreeBSD: releng/11.0/usr.sbin/ndiscvt/Makefile 290084 2015-10-27 23:35:08Z bdrewery $

.PATH:	${.CURDIR}/../../sys/compat/ndis

PROG=	ndiscvt
SRCS=	ndiscvt.c
SRCS+=	subr_pe.c
SRCS+=	inf.c inf-token.l inf-parse.y y.tab.h

MAN=	ndiscvt.8
MAN+=	ndisgen.8

WARNS?=	4
NO_WCAST_ALIGN=

LIBADD=	l

YFLAGS+=-v

CFLAGS+=-I. -I${.CURDIR} -I${.CURDIR}/../../sys

CLEANFILES= y.output

FILES= windrv_stub.c
FILESDIR= ${SHAREDIR}/misc

SCRIPTS= ndisgen.sh

.include <bsd.prog.mk>
