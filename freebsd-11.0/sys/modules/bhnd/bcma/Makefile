# $FreeBSD: releng/11.0/sys/modules/bhnd/bcma/Makefile 296078 2016-02-26 03:34:32Z adrian $

.PATH:	${.CURDIR}/../../../dev/bhnd/bcma

KMOD=	bcma
SRCS=	bcma.c bcma_subr.c bcma_erom.c

SRCS+=	device_if.h bus_if.h bhnd_bus_if.h

.include <bsd.kmod.mk>
