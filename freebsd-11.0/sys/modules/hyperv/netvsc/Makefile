# $FreeBSD: releng/11.0/sys/modules/hyperv/netvsc/Makefile 284969 2015-06-30 19:35:14Z lwhsu $

.PATH:  ${.CURDIR}/../../../dev/hyperv/netvsc

KMOD=	hv_netvsc
SRCS=	hv_net_vsc.c \
	hv_netvsc_drv_freebsd.c \
	hv_rndis_filter.c
SRCS+=	bus_if.h device_if.h opt_inet.h opt_inet6.h

CFLAGS+= -I${.CURDIR}/../../../dev/hyperv/netvsc

.include <bsd.kmod.mk>
