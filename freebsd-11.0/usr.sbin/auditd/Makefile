#
# $FreeBSD: releng/11.0/usr.sbin/auditd/Makefile 275054 2014-11-25 16:57:27Z bapt $
#

OPENBSMDIR=${.CURDIR}/../../contrib/openbsm
.PATH: ${OPENBSMDIR}/bin/auditd

CFLAGS+=	-I${OPENBSMDIR}

PROG=	auditd
SRCS=	auditd.c audit_warn.c auditd_fbsd.c
MAN=	auditd.8

LIBADD=	auditd bsm

WARNS?=	3

.include <bsd.prog.mk>
