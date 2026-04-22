# $FreeBSD: releng/11.0/sys/modules/nctgpio/Makefile 297443 2016-03-31 04:57:38Z adrian $

.PATH:	${.CURDIR}/../../dev/nctgpio
KMOD=	nctgpio
SRCS=	nctgpio.c
SRCS+=	device_if.h bus_if.h isa_if.h gpio_if.h opt_platform.h

.include <bsd.kmod.mk>
