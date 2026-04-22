# $FreeBSD: releng/11.0/sys/modules/ow/owc/Makefile 287225 2015-08-27 23:33:38Z imp $

.PATH: ${.CURDIR}/../../../dev/ow

KMOD=	owc
SRCS=	owc_gpiobus.c
SRCS+=	gpio_if.h gpiobus_if.h owll_if.h ofw_bus_if.h bus_if.h device_if.h
SRCS+=	opt_platform.h

.include <bsd.kmod.mk>
