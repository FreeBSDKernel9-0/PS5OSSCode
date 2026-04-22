#
# $FreeBSD: releng/11.0/usr.sbin/praudit/Makefile 275054 2014-11-25 16:57:27Z bapt $
#

OPENBSMDIR=${.CURDIR}/../../contrib/openbsm
.PATH: ${OPENBSMDIR}/bin/praudit

PROG=	praudit
MAN=	praudit.1

WARNS?=	3

LIBADD=	bsm

.include <bsd.prog.mk>
