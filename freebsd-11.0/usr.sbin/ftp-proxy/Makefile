# $FreeBSD: releng/11.0/usr.sbin/ftp-proxy/Makefile 275054 2014-11-25 16:57:27Z bapt $

.PATH:	${.CURDIR}/../../contrib/pf/ftp-proxy

PROG=	ftp-proxy
MAN=	ftp-proxy.8

SRCS=	ftp-proxy.c filter.c

CFLAGS+=-I${.CURDIR}/../../contrib/pf/libevent

LIBADD=	event

WARNS?=	3

.include <bsd.prog.mk>
