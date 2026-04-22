# $FreeBSD: releng/11.0/sys/modules/vnic/vnicvf/Makefile 300294 2016-05-20 11:00:06Z wma $

SYSDIR?=${.CURDIR}/../../..
.include "${SYSDIR}/conf/kern.opts.mk"

S=	${.CURDIR}/../../..

.PATH: $S/dev/vnic

KMOD=	vnicvf
SRCS=	nicvf_main.c nicvf_queues.c
SRCS+=	opt_platform.h ofw_bus_if.h lmac_if.h miibus_if.h pci_if.h bus_if.h device_if.h opt_inet.h opt_inet6.h

CFLAGS+=	-DFDT -DPCI_IOV

.include <bsd.kmod.mk>
