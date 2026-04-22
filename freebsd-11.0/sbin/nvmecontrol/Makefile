# $FreeBSD: releng/11.0/sbin/nvmecontrol/Makefile 298107 2016-04-16 07:45:30Z gjb $

PACKAGE=runtime
PROG=	nvmecontrol
SRCS=	nvmecontrol.c devlist.c firmware.c identify.c logpage.c	\
	perftest.c reset.c nvme_util.c power.c
MAN=	nvmecontrol.8

.PATH:	${.CURDIR}/../../sys/dev/nvme

.include <bsd.prog.mk>
