#
# $FreeBSD: releng/11.0/usr.sbin/audit/Makefile 275054 2014-11-25 16:57:27Z bapt $
#

OPENBSMDIR=${.CURDIR}/../../contrib/openbsm
.PATH: ${OPENBSMDIR}/bin/audit

CFLAGS+=	-I${OPENBSMDIR}

PROG=	audit
MAN=	audit.8

LIBADD=	bsm

WARNS?=	5

.include <bsd.prog.mk>
