# $FreeBSD: releng/11.0/sys/modules/cardbus/Makefile 277162 2015-01-14 05:41:28Z imp $

.PATH: ${.CURDIR}/../../dev/cardbus

KMOD=	cardbus
SRCS=	cardbus.c cardbus_cis.c cardbus_device.c \
	device_if.h bus_if.h card_if.h power_if.h pci_if.h pcib_if.h

.if ${MACHINE_ARCH} == "i386" || ${MACHINE_ARCH} == "amd64"
CFLAGS += -DNEW_PCIB
.endif

.include <bsd.kmod.mk>
