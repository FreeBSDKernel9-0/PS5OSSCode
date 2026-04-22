# $FreeBSD: releng/11.0/sbin/iscontrol/Makefile 298107 2016-04-16 07:45:30Z gjb $

PACKAGE=iscsi_legacy
SRCS= iscontrol.c pdu.c fsm.c config.c login.c auth_subr.c misc.c
PROG= iscontrol
LIBADD=	cam md
S= ${.CURDIR}/../../sys

WARNS?=	3
CFLAGS+=	-I$S

MAN= iscontrol.8

.include <bsd.prog.mk>
