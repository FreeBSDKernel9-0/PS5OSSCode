# $FreeBSD: releng/11.0/sys/modules/random_fortuna/Makefile 286839 2015-08-17 07:36:12Z markm $

.PATH:	${.CURDIR}/../../dev/random

KMOD	= random_fortuna
SRCS	= randomdev.c hash.c fortuna.c
SRCS	+= opt_param.h bus_if.h device_if.h
SRCS	+= opt_ddb.h
CFLAGS	+= -DRANDOM_LOADABLE

.include <bsd.kmod.mk>
