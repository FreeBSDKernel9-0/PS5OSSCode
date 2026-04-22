# $FreeBSD: releng/11.0/sys/modules/si/Makefile 278959 2015-02-18 15:25:22Z imp $

.PATH: ${.CURDIR}/../../dev/si

KMOD=	si
SRCS=	si.c si2_z280.c si3_t225.c si_isa.c si_pci.c
SRCS+=	bus_if.h device_if.h isa_if.h pci_if.h
SRCS+=	opt_compat.h opt_debug_si.h opt_eisa.h
SRCS.DEV_EISA=si_eisa.c eisa_if.h

.include <bsd.kmod.mk>
