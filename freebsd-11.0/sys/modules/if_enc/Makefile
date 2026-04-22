# $FreeBSD: releng/11.0/sys/modules/if_enc/Makefile 291292 2015-11-25 07:31:59Z ae $

SYSDIR?=${.CURDIR}/../..
.include "${SYSDIR}/conf/kern.opts.mk"

.PATH: ${SYSDIR}/net

KMOD=	if_enc
SRCS=	if_enc.c opt_inet.h opt_inet6.h

.include <bsd.kmod.mk>
