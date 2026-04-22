# $FreeBSD: releng/11.0/sys/modules/padlock_rng/Makefile 273872 2014-10-30 21:21:53Z markm $

.PATH: ${.CURDIR}/../../dev/random

KMOD=	padlock_rng
SRCS=	nehemiah.c
SRCS+=	bus_if.h device_if.h

CFLAGS+= -I${.CURDIR}/../..

.include <bsd.kmod.mk>
