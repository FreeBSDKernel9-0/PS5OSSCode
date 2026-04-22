# $FreeBSD: releng/11.0/sys/modules/linsysfs/Makefile 283421 2015-05-24 15:51:18Z dchagin $

.PATH: ${.CURDIR}/../../compat/linsysfs

KMOD=	linsysfs
SRCS=	vnode_if.h \
	device_if.h bus_if.h  pci_if.h \
	linsysfs.c

.include <bsd.kmod.mk>
