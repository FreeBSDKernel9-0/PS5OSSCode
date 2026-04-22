# $FreeBSD: releng/11.0/sys/modules/proto/Makefile 285068 2015-07-03 01:50:26Z imp $

.PATH: ${.CURDIR}/../../dev/proto

KMOD=	proto
SRCS=	\
	proto_bus_isa.c \
	proto_bus_pci.c \
	proto_busdma.c \
	proto_core.c

SRCS+=	\
	bus_if.h \
	device_if.h \
	isa_if.h \
	pci_if.h

.include <bsd.kmod.mk>
