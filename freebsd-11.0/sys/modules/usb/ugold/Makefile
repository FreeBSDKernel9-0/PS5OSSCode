#
# $FreeBSD: releng/11.0/sys/modules/usb/ugold/Makefile 284272 2015-06-11 15:23:47Z hselasky $
#

S=     ${.CURDIR}/../../..

.PATH: $S/dev/usb/misc

KMOD=	ugold
SRCS=	opt_bus.h opt_usb.h device_if.h bus_if.h usb_if.h vnode_if.h \
	usbdevs.h ugold.c

.include <bsd.kmod.mk>
