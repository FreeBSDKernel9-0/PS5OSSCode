# $FreeBSD: releng/11.0/cddl/lib/libavl/Makefile 302141 2016-06-23 15:02:57Z asomers $

.PATH: ${.CURDIR}/../../../sys/cddl/contrib/opensolaris/common/avl

LIB=	avl
SRCS=	avl.c
WARNS?=	3
CFLAGS+= -I${.CURDIR}/../../../sys/cddl/compat/opensolaris
CFLAGS+= -I${.CURDIR}/../../../sys/cddl/contrib/opensolaris/uts/common

.include <bsd.lib.mk>
