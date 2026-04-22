# $FreeBSD: releng/11.0/usr.sbin/apmd/Makefile 298107 2016-04-16 07:45:30Z gjb $

PROG=	apmd
MAN=	apmd.8
MANSUBDIR= /i386
SRCS=	apmd.c apmdlex.l apmdparse.y y.tab.h

PACKAGE=apm

WARNS?=	3

LIBADD=	l

CFLAGS+= -I${.CURDIR}

test:
	./apmd -d -f etc/apmd.conf -n

.include <bsd.prog.mk>
