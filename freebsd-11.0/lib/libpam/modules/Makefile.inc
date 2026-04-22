# $FreeBSD: releng/11.0/lib/libpam/modules/Makefile.inc 297946 2016-04-14 01:17:03Z bdrewery $

PAMDIR=		${.CURDIR}/../../../../contrib/openpam

MK_INSTALLLIB=	no
MK_PROFILE=	no

CFLAGS+= -I${PAMDIR}/include -I${.CURDIR}/../../libpam

SHLIB_NAME?=	${LIB}.so.${SHLIB_MAJOR}
LIBADD+=	pam

.include "../Makefile.inc"
