# $FreeBSD: releng/11.0/sys/modules/tsec/Makefile 269133 2014-07-26 17:59:25Z marcel $

SYSDIR?=${.CURDIR}/../..

.PATH:	${SYSDIR}/dev/tsec

KMOD=	if_tsec
SRCS=	if_tsec.c if_tsec_fdt.c
SRCS+=	bus_if.h device_if.h miibus_if.h miidevs.h ofw_bus_if.h

.include <bsd.kmod.mk>
