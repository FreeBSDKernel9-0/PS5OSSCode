
# $FreeBSD: releng/11.0/sys/modules/puc/Makefile 285068 2015-07-03 01:50:26Z imp $

.PATH:	${.CURDIR}/../../dev/puc

KMOD=	puc
SRCS=	puc.c puc_cfg.c puc_pci.c puc_pccard.c pucdata.c
SRCS+=	bus_if.h device_if.h serdev_if.c serdev_if.h \
	card_if.h pci_if.h

.include <bsd.kmod.mk>
