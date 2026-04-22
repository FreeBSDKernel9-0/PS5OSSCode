# $FreeBSD: releng/11.0/sys/modules/siba_bwn/Makefile 301692 2016-06-08 20:01:10Z avos $

.PATH: ${.CURDIR}/../../dev/siba

KMOD=	siba_bwn
SRCS=	siba_core.c siba_bwn.c sibareg.h sibavar.h
SRCS+=	device_if.h bus_if.h pci_if.h opt_siba.h

.include <bsd.kmod.mk>
