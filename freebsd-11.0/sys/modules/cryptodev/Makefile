# $FreeBSD: releng/11.0/sys/modules/cryptodev/Makefile 291153 2015-11-22 02:01:01Z markj $

.PATH:	${.CURDIR}/../../opencrypto
KMOD	= cryptodev
SRCS	= cryptodev.c
SRCS	+= bus_if.h device_if.h opt_compat.h

.include <bsd.kmod.mk>
