# $FreeBSD: releng/11.0/sys/modules/sound/driver/maestro/Makefile 267297 2014-06-09 20:48:38Z jhb $

.PATH: ${.CURDIR}/../../../../dev/sound/pci

KMOD=	snd_maestro
SRCS=	device_if.h bus_if.h pci_if.h
SRCS+=	maestro.c

.include <bsd.kmod.mk>
