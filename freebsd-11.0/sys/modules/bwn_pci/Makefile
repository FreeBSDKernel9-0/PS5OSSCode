# $FreeBSD: releng/11.0/sys/modules/bwn_pci/Makefile 301692 2016-06-08 20:01:10Z avos $

.PATH: ${.CURDIR}/../../dev/bwn

KMOD=	if_bwn_pci
SRCS=	if_bwn_pci.c bwn_mac.c
SRCS+=	bhnd_bus_if.h bhndb_bus_if.h \
	bhndb_if.h bhnd_nvram_map.h
SRCS+=	device_if.h bus_if.h pci_if.h \
	opt_bwn.h opt_wlan.h opt_global.h

.include <bsd.kmod.mk>
