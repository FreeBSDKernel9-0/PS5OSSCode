# $FreeBSD: releng/11.0/sys/modules/vnic/thunder_mdio/Makefile 300294 2016-05-20 11:00:06Z wma $

SYSDIR?=${.CURDIR}/../../..
.include "${SYSDIR}/conf/kern.opts.mk"

S=	${.CURDIR}/../../..

.PATH: $S/dev/vnic

KMOD=	thunder_mdio
SRCS=	opt_platform.h device_if.h bus_if.h pci_if.h ofw_bus_if.h miibus_if.h lmac_if.h
SRCS+=	thunder_mdio.c thunder_mdio_fdt.c

CFLAGS+=	-DFDT

.include <bsd.kmod.mk>
