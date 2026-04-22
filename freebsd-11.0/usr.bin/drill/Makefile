# $FreeBSD: releng/11.0/usr.bin/drill/Makefile 275042 2014-11-25 14:29:10Z bapt $

# Vendor sources and generated files
LDNSDIR= ${.CURDIR}/../../contrib/ldns

.PATH: ${LDNSDIR}/drill

PROG=	drill
SRCS=	drill.c drill_util.c error.c root.c work.c \
	chasetrace.c dnssec.c securetrace.c
CFLAGS+= -I${LDNSDIR}
NO_WERROR= true
MAN=	drill.1

LIBADD=	ldns crypto

.include <bsd.prog.mk>
