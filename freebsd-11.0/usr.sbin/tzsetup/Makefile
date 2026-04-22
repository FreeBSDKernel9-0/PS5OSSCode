# $FreeBSD: releng/11.0/usr.sbin/tzsetup/Makefile 275054 2014-11-25 16:57:27Z bapt $

PROG=	tzsetup
MAN=	tzsetup.8

CFLAGS+= -I${.CURDIR}/../../contrib/dialog -I.

WARNS?=	3

LIBADD=	dialog ncursesw

.include <bsd.prog.mk>
