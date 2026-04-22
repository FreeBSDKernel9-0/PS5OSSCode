# $Id: Makefile,v 1.2 2004/02/13 21:44:41 max Exp $
# $FreeBSD: releng/11.0/usr.sbin/bluetooth/bthidcontrol/Makefile 275054 2014-11-25 16:57:27Z bapt $

.PATH:	${.CURDIR}/../bthidd

PROG=		bthidcontrol
MAN=		bthidcontrol.8
SRCS=		bthidcontrol.c hid.c lexer.l  parser.y sdp.c
WARNS?=		1
CFLAGS+=	-DBTHIDCONTROL=1 -I${.CURDIR}/../bthidd

LIBADD+=	bluetooth sdp usbhid

.include <bsd.prog.mk>
