# $FreeBSD: releng/11.0/sys/modules/usb/rsu/Makefile 288352 2015-09-29 04:56:27Z adrian $

.PATH: ${.CURDIR}/../../../dev/usb/wlan

KMOD    = if_rsu
SRCS    = if_rsu.c if_rsureg.h \
	  bus_if.h device_if.h \
	  opt_bus.h opt_usb.h opt_wlan.h usb_if.h usbdevs.h

.include <bsd.kmod.mk>
