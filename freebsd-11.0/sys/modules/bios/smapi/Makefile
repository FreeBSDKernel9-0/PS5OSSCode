# $FreeBSD: releng/11.0/sys/modules/bios/smapi/Makefile 276696 2015-01-05 12:28:22Z imp $
#

.PATH:  ${.CURDIR}/../../../i386/bios

KMOD=	smapi
SRCS=	smapi.c smapi_bios.S \
	bus_if.h device_if.h

.include <bsd.kmod.mk>

CFLAGS.smapi_bios.S=	${CLANG_NO_IAS34}
