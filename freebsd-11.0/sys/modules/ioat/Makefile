# ioat Loadable Kernel Module
#
# $FreeBSD: releng/11.0/sys/modules/ioat/Makefile 287117 2015-08-24 19:32:03Z cem $

IOAT_SRC_PATH = ${.CURDIR}/../..

.PATH:  ${IOAT_SRC_PATH}/dev/ioat

KMOD=	ioat
SRCS=	ioat.c ioat_test.c
SRCS+=	device_if.h bus_if.h pci_if.h

CFLAGS+=	-I${IOAT_SRC_PATH}

.include <bsd.kmod.mk>
