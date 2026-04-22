# $FreeBSD: releng/11.0/sys/modules/ep/Makefile 278959 2015-02-18 15:25:22Z imp $

SYSDIR?=${.CURDIR}/../..
.include "${SYSDIR}/conf/kern.opts.mk"

.PATH: ${SYSDIR}/dev/ep

KMOD=	if_ep
SRCS=	if_ep.c
SRCS+=	if_ep_pccard.c pccarddevs.h card_if.h
SRCS+=	if_ep_isa.c isa_if.h
SRCS.DEV_EISA=	if_ep_eisa.c eisa_if.h
SRCS+=	bus_if.h device_if.h

.include <bsd.kmod.mk>
