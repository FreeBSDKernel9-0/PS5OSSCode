#	From: @(#)Makefile	8.1 (Berkeley) 6/6/93
# $FreeBSD: releng/11.0/usr.sbin/lpr/lpc/Makefile 275054 2014-11-25 16:57:27Z bapt $

.PATH: ${.CURDIR}/../common_source

PROG=	lpc
MAN=	lpc.8
SRCS=	lpc.c cmds.c cmdtab.c movejobs.c
BINGRP=	daemon
BINMODE= 2555

CFLAGS+= -I${.CURDIR}/../common_source

WARNS?=	0

LIBADD=	lpr edit

.include <bsd.prog.mk>
