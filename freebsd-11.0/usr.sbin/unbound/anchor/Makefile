# $FreeBSD: releng/11.0/usr.sbin/unbound/anchor/Makefile 296677 2016-03-11 20:04:32Z jhibbits $

# Vendor sources and generated files
LDNSDIR= ${.CURDIR}/../../../contrib/ldns
UNBOUNDDIR= ${.CURDIR}/../../../contrib/unbound
EXPATDIR= ${.CURDIR}/../../../contrib/expat

.PATH: ${UNBOUNDDIR} ${UNBOUNDDIR}/smallapp ${UNBOUNDDIR}/doc

PROG=	unbound-anchor
SRCS=	unbound-anchor.c
CFLAGS+=	-I${UNBOUNDDIR} -I${LDNSDIR} -I${EXPATDIR}/lib
LIBADD=	unbound bsdxml ssl crypto pthread
MAN=	unbound-anchor.8

.include <bsd.prog.mk>
