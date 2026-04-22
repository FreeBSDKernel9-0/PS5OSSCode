#
# $FreeBSD: releng/11.0/lib/libauditd/Makefile 298107 2016-04-16 07:45:30Z gjb $
#

PACKAGE=lib${LIB}
OPENBSMDIR=		${.CURDIR}/../../contrib/openbsm
_LIBAUDITDDIR=		${OPENBSMDIR}/libauditd
_LIBBSMDIR=		${OPENBSMDIR}/libbsm

LIB=		auditd

.PATH:		${_LIBAUDITDDIR}

SRCS=	auditd_lib.c

#
# Must use BSM include files from within the contrib area, not the system.
#
CFLAGS+=	-I${OPENBSMDIR} -I${_LIBBSMDIR}

WARNS?=		3

MAN=

.include <bsd.lib.mk>
