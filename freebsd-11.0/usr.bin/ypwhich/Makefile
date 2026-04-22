#	from: @(#)Makefile	5.8 (Berkeley) 7/28/90
# $FreeBSD: releng/11.0/usr.bin/ypwhich/Makefile 287129 2015-08-25 15:27:32Z araujo $

YPSERV=${.CURDIR}/../../usr.sbin/ypserv/common
.PATH: ${YPSERV}

PROG=	ypwhich
SRCS=	yplib_host.c ypwhich.c

CFLAGS+=	-I${YPSERV} -I.

WARNS?=	2

.include <bsd.prog.mk>
