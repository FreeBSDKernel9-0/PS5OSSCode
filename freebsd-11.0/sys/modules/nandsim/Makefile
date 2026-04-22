# $FreeBSD: releng/11.0/sys/modules/nandsim/Makefile 285068 2015-07-03 01:50:26Z imp $

.PATH: ${.CURDIR}/../../dev/nand

KMOD=	nandsim
SRCS=	nandsim.c nandsim_chip.c nandsim_swap.c nandsim_ctrl.c nandsim_log.c\
	bus_if.h device_if.h vnode_if.h nfc_if.h nand_if.h

.include <bsd.kmod.mk>
