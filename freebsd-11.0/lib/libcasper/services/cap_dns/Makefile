# $FreeBSD: releng/11.0/lib/libcasper/services/cap_dns/Makefile 298107 2016-04-16 07:45:30Z gjb $

PACKAGE=libcasper
LIB=	cap_dns

SHLIB_MAJOR=	0
SHLIBDIR?=	/lib/casper
INCSDIR?=	${INCLUDEDIR}/casper

SRCS=	cap_dns.c

INCS=	cap_dns.h

LIBADD=	nv

CFLAGS+=-I${.CURDIR}

.include <bsd.lib.mk>
