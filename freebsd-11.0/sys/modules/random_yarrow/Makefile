# $FreeBSD: releng/11.0/sys/modules/random_yarrow/Makefile 286839 2015-08-17 07:36:12Z markm $

.PATH:	${.CURDIR}/../../dev/random

KMOD	= random_yarrow
SRCS	= randomdev.c hash.c yarrow.c
SRCS	+= opt_param.h bus_if.h device_if.h
SRCS	+= opt_ddb.h
CFLAGS	+= -DRANDOM_LOADABLE

.include <bsd.kmod.mk>
