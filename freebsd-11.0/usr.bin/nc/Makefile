# $FreeBSD: releng/11.0/usr.bin/nc/Makefile 275042 2014-11-25 14:29:10Z bapt $

.PATH: ${.CURDIR}/../../contrib/netcat

PROG=	nc
SRCS=	netcat.c atomicio.c socks.c

CFLAGS+=-DIPSEC
LIBADD=	ipsec

WARNS?=	2

.include <bsd.prog.mk>
