# $FreeBSD: releng/11.0/lib/libalias/libalias/Makefile 298107 2016-04-16 07:45:30Z gjb $

.PATH: ${.CURDIR}/../../../sys/netinet/libalias

PACKAGE=lib${LIB}
LIB=	alias
SHLIBDIR?= /lib
SHLIB_MAJOR= 7
MAN=	libalias.3
SRCS=	alias.c alias_db.c alias_proxy.c alias_util.c alias_mod.c
INCS=	alias.h
NO_WERROR=

.include <bsd.lib.mk>
