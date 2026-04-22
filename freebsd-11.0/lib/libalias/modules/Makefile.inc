# $FreeBSD: releng/11.0/lib/libalias/modules/Makefile.inc 284421 2015-06-15 19:28:07Z bapt $

.PATH: ${.CURDIR}/../../../../sys/netinet/libalias

SHLIBDIR?= /lib
LIB?=   alias_${NAME}
SHLIB_NAME?=libalias_${NAME}.so
WARNS?=	1
