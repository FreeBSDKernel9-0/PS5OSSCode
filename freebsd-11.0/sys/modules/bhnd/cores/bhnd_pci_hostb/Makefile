# $FreeBSD: releng/11.0/sys/modules/bhnd/cores/bhnd_pci_hostb/Makefile 300015 2016-05-17 06:52:53Z adrian $

.PATH: ${.CURDIR}/../../../../dev/bhnd/cores/pci
.PATH: ${.CURDIR}/../../../../dev/bhnd/cores/pcie2

KMOD=	bhnd_pci_hostb
SRCS=	bhnd_pci_hostb.c bhnd_pcie2_hostb.c
SRCS+=	device_if.h bus_if.h pci_if.h \
	bhnd_bus_if.h bhnd_chipc_if.h

.include <bsd.kmod.mk>
