# $FreeBSD: releng/11.0/sys/modules/geom/geom_part/geom_part_bsd/Makefile 285068 2015-07-03 01:50:26Z imp $

.PATH: ${.CURDIR}/../../../../geom/part ${.CURDIR}/../../../../geom

KMOD=	geom_part_bsd
SRCS=	g_part_bsd.c geom_bsd_enc.c

SRCS+=	bus_if.h device_if.h g_part_if.h

.include <bsd.kmod.mk>
