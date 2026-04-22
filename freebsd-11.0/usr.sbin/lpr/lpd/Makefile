#	From: @(#)Makefile	8.1 (Berkeley) 6/6/93
# $FreeBSD: releng/11.0/usr.sbin/lpr/lpd/Makefile 275054 2014-11-25 16:57:27Z bapt $

PROG=	lpd
MAN=	lpd.8
SRCS=	lpd.c printjob.c recvjob.c lpdchar.c modes.c

CFLAGS+= -I${.CURDIR}/../common_source

WARNS?=	1

LIBADD=	lpr

.include <bsd.prog.mk>
