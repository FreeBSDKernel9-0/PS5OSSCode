# $FreeBSD: releng/11.0/sys/modules/if_gre/Makefile 278960 2015-02-18 15:25:25Z imp $

SYSDIR?=${.CURDIR}/../..
.PATH: ${SYSDIR}/net ${SYSDIR}/netinet ${SYSDIR}/netinet6
.include "${SYSDIR}/conf/kern.opts.mk"

KMOD=	if_gre
SRCS=	if_gre.c opt_inet.h opt_inet6.h
SRCS.INET=	ip_gre.c
SRCS.INET6=	ip6_gre.c

.include <bsd.kmod.mk>
