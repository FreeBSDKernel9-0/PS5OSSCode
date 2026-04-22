# $FreeBSD: releng/11.0/sys/modules/ncr/Makefile 272442 2014-10-02 18:45:00Z rpaulo $

.PATH: ${.CURDIR}/../../dev/ncr

KMOD=	ncr
SRCS=	ncr.c
SRCS+=	device_if.h bus_if.h pci_if.h opt_ncr.h opt_cam.h

.include <bsd.kmod.mk>
