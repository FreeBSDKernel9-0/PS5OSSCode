# $FreeBSD: releng/11.0/sys/modules/vnic/thunder_bgx/Makefile 300294 2016-05-20 11:00:06Z wma $

SYSDIR?=${.CURDIR}/../../..
.include "${SYSDIR}/conf/kern.opts.mk"

S=	${.CURDIR}/../../..

.PATH: $S/dev/vnic

KMOD=	thunder_bgx
SRCS=	thunder_bgx.c thunder_bgx_fdt.c
SRCS+=	opt_platform.h device_if.h bus_if.h pci_if.h lmac_if.h ofw_bus_if.h

CFLAGS+=	-DFDT

.include <bsd.kmod.mk>
