#
# $FreeBSD: releng/11.0/sys/modules/gpio/gpiokeys/Makefile 299740 2016-05-14 09:39:21Z bz $
#

.PATH:	${.CURDIR}/../../../dev/gpio/

KMOD=	gpiokeys
SRCS=	gpiokeys.c gpiokeys_codes.c
SRCS+=	bus_if.h device_if.h gpio_if.h ofw_bus_if.h
SRCS+=	opt_platform.h opt_kbd.h

CFLAGS+=  -I. -I${.CURDIR}/../../../dev/gpio/

.include <bsd.kmod.mk>
