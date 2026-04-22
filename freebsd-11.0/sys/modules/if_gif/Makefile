# $FreeBSD: releng/11.0/sys/modules/if_gif/Makefile 278960 2015-02-18 15:25:25Z imp $

SYSDIR?=${.CURDIR}/../..
.include "${SYSDIR}/conf/kern.opts.mk"

.PATH: ${SYSDIR}/net ${SYSDIR}/netinet ${SYSDIR}/netinet6

KMOD=	if_gif
SRCS=	if_gif.c opt_inet.h opt_inet6.h
SRCS.INET=in_gif.c
SRCS.INET6=in6_gif.c

.include <bsd.kmod.mk>
