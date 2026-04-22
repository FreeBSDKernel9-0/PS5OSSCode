# $FreeBSD: releng/11.0/sbin/mksnap_ffs/Makefile 298107 2016-04-16 07:45:30Z gjb $

.PATH:	${.CURDIR}/../mount

PACKAGE=runtime
PROG=	mksnap_ffs
SRCS=	mksnap_ffs.c getmntopts.c
MAN=	mksnap_ffs.8

WARNS?=	2
CFLAGS+=-I${.CURDIR}/../mount

.if defined(NOSUID)
BINMODE=554
.else
BINMODE=4554
BINOWN=	root
.endif
BINGRP=	operator

.include <bsd.prog.mk>
