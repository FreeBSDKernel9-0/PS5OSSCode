# $FreeBSD: releng/11.0/usr.sbin/bhyveload/Makefile 298107 2016-04-16 07:45:30Z gjb $

PROG=	bhyveload
SRCS=	bhyveload.c
MAN=	bhyveload.8
PACKAGE=	bhyve

LIBADD=	vmmapi

WARNS?=	3

CFLAGS+=-I${.CURDIR}/../../sys/boot/userboot

.include <bsd.prog.mk>
