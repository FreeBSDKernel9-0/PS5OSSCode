# $FreeBSD: releng/11.0/sys/modules/carp/Makefile 269540 2014-08-04 22:37:02Z imp $

.PATH: ${.CURDIR}/../../netinet
.PATH: ${.CURDIR}/../../crypto

KMOD=	carp
SRCS=	ip_carp.c sha1.c
SRCS+=	device_if.h bus_if.h vnode_if.h
SRCS+=	opt_carp.h opt_bpf.h opt_inet.h opt_inet6.h opt_ofed.h

.include <bsd.kmod.mk>
