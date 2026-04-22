#	From: @(#)Makefile	8.1 (Berkeley) 6/6/93
# $FreeBSD: releng/11.0/usr.sbin/lpr/lpq/Makefile 275054 2014-11-25 16:57:27Z bapt $

BINDIR=	/usr/bin

PROG=	lpq
BINOWN=	root
BINGRP=	daemon
BINMODE= 6555

CFLAGS+= -I${.CURDIR}/../common_source

LIBADD=	lpr

.include <bsd.prog.mk>
