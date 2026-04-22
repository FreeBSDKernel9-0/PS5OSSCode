# $FreeBSD: releng/11.0/sys/modules/sound/driver/uaudio/Makefile 266006 2014-05-14 07:33:06Z hselasky $

S=	${.CURDIR}/../../../..

.PATH:	$S/dev/sound/usb

KMOD=	snd_uaudio
SRCS=	bus_if.h device_if.h usb_if.h vnode_if.h
SRCS+=	opt_usb.h opt_bus.h feeder_if.h channel_if.h usbdevs.h
SRCS+=	uaudio_pcm.c uaudio.c 

.include <bsd.kmod.mk>
