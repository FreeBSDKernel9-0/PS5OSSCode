# $FreeBSD: releng/11.0/sys/modules/rl/Makefile 271864 2014-09-19 10:32:20Z glebius $

.PATH: ${.CURDIR}/../../dev/rl

KMOD=	if_rl
SRCS=	if_rl.c device_if.h bus_if.h pci_if.h
SRCS+=	miibus_if.h

.include <bsd.kmod.mk>
