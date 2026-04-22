# $FreeBSD: releng/11.0/sys/modules/hyperv/utilities/Makefile 295958 2016-02-24 05:01:18Z sephe $

.PATH:	${.CURDIR}/../../../dev/hyperv/utilities

KMOD=	hv_utils
SRCS=	hv_util.c hv_kvp.c hv_timesync.c hv_shutdown.c hv_heartbeat.c
SRCS+=	bus_if.h device_if.h

CFLAGS+= -I${.CURDIR}/../../../dev/hyperv/include \
	 -I${.CURDIR}/../../../dev/hyperv/vmbus

.include <bsd.kmod.mk>
