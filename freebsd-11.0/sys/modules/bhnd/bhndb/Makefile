# $FreeBSD: releng/11.0/sys/modules/bhnd/bhndb/Makefile 300445 2016-05-23 03:47:44Z adrian $

.PATH: ${.CURDIR}/../../../dev/bhnd/bhndb

KMOD=	bhndb
SRCS=	bhndb.c bhndb_subr.c bhndb_hwdata.c \
	bhnd_bhndb.c \
	bhndb_bus_if.c bhndb_bus_if.h \
	bhndb_if.c bhndb_if.h
SRCS+=	bhnd_bus_if.h \
	bhnd_chipc_if.h \
	bhnd_nvram_if.h

SRCS+=	device_if.h bus_if.h pci_if.h

.include <bsd.kmod.mk>
