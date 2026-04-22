# $FreeBSD: releng/11.0/sys/modules/ahci/Makefile 292001 2015-12-08 20:05:27Z andrew $

.PATH: ${.CURDIR}/../../dev/ahci

KMOD=	ahci
SRCS=	ahci.c ahci_pci.c ahciem.c ahci.h device_if.h bus_if.h pci_if.h opt_cam.h

.if ${MACHINE_CPUARCH} == "aarch64"
SRCS+=	ahci_generic.c ofw_bus_if.h
.endif

.include <bsd.kmod.mk>
