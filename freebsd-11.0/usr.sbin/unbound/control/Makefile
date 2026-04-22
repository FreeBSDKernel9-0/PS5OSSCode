# $FreeBSD: releng/11.0/usr.sbin/unbound/control/Makefile 296677 2016-03-11 20:04:32Z jhibbits $

# Vendor sources and generated files
LDNSDIR= ${.CURDIR}/../../../contrib/ldns
UNBOUNDDIR= ${.CURDIR}/../../../contrib/unbound

.PATH: ${UNBOUNDDIR} ${UNBOUNDDIR}/smallapp ${UNBOUNDDIR}/doc

PROG=	unbound-control
SRCS=	unbound-control.c worker_cb.c
CFLAGS+=	-I${UNBOUNDDIR} -I${LDNSDIR}
LIBADD=	unbound crypto ssl pthread
MAN=	unbound-control.8

.include <bsd.prog.mk>
