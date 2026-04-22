# $FreeBSD: releng/11.0/sys/modules/bhnd/siba/Makefile 296078 2016-02-26 03:34:32Z adrian $

.PATH:	${.CURDIR}/../../../dev/bhnd/siba

KMOD=	siba
SRCS=	siba.c siba_subr.c

SRCS+=	device_if.h bus_if.h bhnd_bus_if.h

.include <bsd.kmod.mk>
