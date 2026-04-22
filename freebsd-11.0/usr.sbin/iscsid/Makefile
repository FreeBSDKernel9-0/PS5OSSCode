# $FreeBSD: releng/11.0/usr.sbin/iscsid/Makefile 300504 2016-05-23 12:58:24Z trasz $

PACKAGE=	iscsi
PROG=		iscsid
SRCS=		chap.c discovery.c iscsid.c keys.c log.c login.c pdu.c
CFLAGS+=	-I${.CURDIR}
CFLAGS+=	-I${.CURDIR}/../../sys/cam
CFLAGS+=	-I${.CURDIR}/../../sys/dev/iscsi
CFLAGS+=	-DICL_KERNEL_PROXY
MAN=		iscsid.8

LIBADD=		md util

WARNS=		6

.include <bsd.prog.mk>
