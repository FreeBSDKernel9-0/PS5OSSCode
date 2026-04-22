# $FreeBSD: releng/11.0/usr.sbin/lpr/chkprintcap/Makefile 275054 2014-11-25 16:57:27Z bapt $

.PATH: ${.CURDIR}/../common_source

PROG=	chkprintcap
MAN=	chkprintcap.8
SRCS=	chkprintcap.c skimprintcap.c

CFLAGS+= -I${.CURDIR}/../common_source

LIBADD=	lpr

.include <bsd.prog.mk>
