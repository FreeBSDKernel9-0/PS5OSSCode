# $FreeBSD: releng/11.0/lib/libcasper/services/cap_pwd/Makefile 298107 2016-04-16 07:45:30Z gjb $

PACKAGE=libcasper
LIB=	cap_pwd

SHLIB_MAJOR=	0
SHLIBDIR?=	/lib/casper
INCSDIR?=	${INCLUDEDIR}/casper

SRCS=	cap_pwd.c

INCS=	cap_pwd.h

LIBADD=	nv

CFLAGS+=-I${.CURDIR}

.include <bsd.lib.mk>
