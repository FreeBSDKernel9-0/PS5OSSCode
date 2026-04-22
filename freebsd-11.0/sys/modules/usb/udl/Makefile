# $FreeBSD: releng/11.0/sys/modules/usb/udl/Makefile 278799 2015-02-15 12:02:17Z hselasky $

S=     ${.CURDIR}/../../..

.PATH: $S/dev/usb/video

KMOD=	udl
SRCS=	opt_bus.h opt_usb.h fb_if.h device_if.h \
	bus_if.h usb_if.h vnode_if.h usbdevs.h \
	udl.c

.include <bsd.kmod.mk>
