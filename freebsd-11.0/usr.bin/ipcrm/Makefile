# $FreeBSD: releng/11.0/usr.bin/ipcrm/Makefile 275042 2014-11-25 14:29:10Z bapt $

PROG=	ipcrm
SRCS=	ipcrm.c ipc.c
LIBADD=	kvm
CFLAGS+=-I${.CURDIR}/../ipcs
.PATH:  ${.CURDIR}/../ipcs

WARNS?=	0

.include <bsd.prog.mk>
