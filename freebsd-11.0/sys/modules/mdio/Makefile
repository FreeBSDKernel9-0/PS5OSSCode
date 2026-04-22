# $FreeBSD: releng/11.0/sys/modules/mdio/Makefile 292738 2015-12-26 02:31:39Z adrian $

.PATH: ${.CURDIR}/../../dev/mdio

KMOD=	mdio
SRCS=	mdio.c
SRCS+=	mdio_if.c mdio_if.h
SRCS+=	device_if.h bus_if.h

.include <bsd.kmod.mk>
