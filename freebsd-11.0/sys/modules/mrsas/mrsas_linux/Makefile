# Makefile for mrsas driver
# $FreeBSD: releng/11.0/sys/modules/mrsas/mrsas_linux/Makefile 272738 2014-10-08 09:30:35Z kadesai $

.PATH: ${.CURDIR}/../../../dev/mrsas

KMOD=	mrsas_linux
SRCS=	mrsas_linux.c
SRCS+=	device_if.h bus_if.h pci_if.h

.include <bsd.kmod.mk>
