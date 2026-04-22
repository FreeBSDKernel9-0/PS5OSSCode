# $FreeBSD: releng/11.0/usr.sbin/ntp/libopts/Makefile 280849 2015-03-30 13:30:15Z cy $

.PATH: ${.CURDIR}/../../../contrib/ntp/sntp/libopts 

LIB=	opts
INTERNALLIB=

SRCS=	libopts.c

CFLAGS+= -I${.CURDIR}/../../../contrib/ntp/include \
	-I${.CURDIR}/../../../contrib/ntp/sntp/libopts \
	-I${.CURDIR}/../

.include <bsd.lib.mk>
