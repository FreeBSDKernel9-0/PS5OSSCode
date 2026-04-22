#	From: @(#)Makefile	8.3 (Berkeley) 1/25/94
# $FreeBSD: releng/11.0/usr.sbin/mountd/Makefile 275054 2014-11-25 16:57:27Z bapt $

PROG=	mountd
SRCS=	mountd.c getmntopts.c
MAN=	exports.5 netgroup.5 mountd.8

MOUNT=  ${.CURDIR}/../../sbin/mount
CFLAGS+= -I${MOUNT}
WARNS?= 2

.PATH: ${MOUNT}

LIBADD=	util

.include <bsd.prog.mk>
