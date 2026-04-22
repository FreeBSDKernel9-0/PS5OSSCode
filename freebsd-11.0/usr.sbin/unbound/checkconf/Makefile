# $FreeBSD: releng/11.0/usr.sbin/unbound/checkconf/Makefile 296677 2016-03-11 20:04:32Z jhibbits $

# Vendor sources and generated files
LDNSDIR= ${.CURDIR}/../../../contrib/ldns
UNBOUNDDIR= ${.CURDIR}/../../../contrib/unbound

.PATH: ${UNBOUNDDIR} ${UNBOUNDDIR}/smallapp ${UNBOUNDDIR}/doc

PROG=	unbound-checkconf
SRCS=	unbound-checkconf.c worker_cb.c
CFLAGS+=	-I${UNBOUNDDIR} -I${LDNSDIR}
LIBADD=	unbound pthread
MAN=	unbound-checkconf.8

.include <bsd.prog.mk>
