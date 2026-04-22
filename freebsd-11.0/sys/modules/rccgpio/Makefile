# $FreeBSD: releng/11.0/sys/modules/rccgpio/Makefile 286908 2015-08-18 21:05:56Z loos $

.PATH:	${.CURDIR}/../../dev/rccgpio
KMOD=	rccgpio
SRCS=	rccgpio.c
SRCS+=	device_if.h bus_if.h isa_if.h gpio_if.h opt_platform.h

.include <bsd.kmod.mk>
