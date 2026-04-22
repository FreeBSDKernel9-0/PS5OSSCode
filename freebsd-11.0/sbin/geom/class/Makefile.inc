# $FreeBSD: releng/11.0/sbin/geom/class/Makefile.inc 284421 2015-06-15 19:28:07Z bapt $

SHLIBDIR?=${GEOM_CLASS_DIR}
SHLIB_NAME?=geom_${GEOM_CLASS}.so
LINKS=	${BINDIR}/geom ${BINDIR}/g${GEOM_CLASS}
MAN=	g${GEOM_CLASS}.8
SRCS+=	geom_${GEOM_CLASS}.c subr.c

NO_WMISSING_VARIABLE_DECLARATIONS=

CFLAGS+= -I${.CURDIR}/../..

.include "../Makefile.inc"
