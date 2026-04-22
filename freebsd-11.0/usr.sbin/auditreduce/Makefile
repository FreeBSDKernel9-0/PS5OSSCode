#
# $FreeBSD: releng/11.0/usr.sbin/auditreduce/Makefile 275054 2014-11-25 16:57:27Z bapt $
#

OPENBSMDIR=${.CURDIR}/../../contrib/openbsm
.PATH: ${OPENBSMDIR}/bin/auditreduce

CFLAGS+=	-I${OPENBSMDIR}

PROG=	auditreduce
MAN=	auditreduce.1

LIBADD=	bsm

WARNS?=	3

.include <bsd.prog.mk>
