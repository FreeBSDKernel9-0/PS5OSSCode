#
# $FreeBSD: releng/11.0/usr.sbin/bhyvectl/Makefile 298107 2016-04-16 07:45:30Z gjb $
#

PROG=	bhyvectl
SRCS=	bhyvectl.c
PACKAGE=	bhyve

MAN=	bhyvectl.8

LIBADD=	vmmapi util

WARNS?=	3

CFLAGS+= -I${.CURDIR}/../../sys/amd64/vmm

.include <bsd.prog.mk>
