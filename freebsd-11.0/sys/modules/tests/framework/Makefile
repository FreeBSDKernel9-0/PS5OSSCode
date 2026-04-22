#
# $FreeBSD: releng/11.0/sys/modules/tests/framework/Makefile 299094 2016-05-04 23:20:53Z ngie $
#

.PATH: ${.CURDIR}/../../../tests/framework

KMOD=	kern_testfrwk
SRCS=	kern_testfrwk.c
SRCS+=	bus_if.h device_if.h

#
# Enable full debugging
#
#CFLAGS += -g

.include <bsd.kmod.mk>
