# $FreeBSD: releng/11.0/sys/modules/bhnd/bhndb_pci/Makefile 299241 2016-05-08 19:14:05Z adrian $

.PATH: ${.CURDIR}/../../../dev/bhnd/bhndb

KMOD=	bhndb_pci
SRCS=	bhndb_pci.c bhndb_pci_hwdata.c \
	bhndb_pci_sprom.c
SRCS+=	bhnd_bus_if.h bhndb_bus_if.h bhndb_if.h
SRCS+=	bhnd_nvram_if.h

SRCS+=	device_if.h bus_if.h pci_if.h

.include <bsd.kmod.mk>
