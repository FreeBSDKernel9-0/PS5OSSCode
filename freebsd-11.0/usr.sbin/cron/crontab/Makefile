# $FreeBSD: releng/11.0/usr.sbin/cron/crontab/Makefile 275054 2014-11-25 16:57:27Z bapt $

BINDIR=	/usr/bin

PROG=	crontab
MAN=	crontab.1 crontab.5
BINOWN=	root
BINMODE=4555
PRECIOUSPROG=

WARNS?=	3

CFLAGS+= -I${.CURDIR}/../cron

LIBADD=	cron md util

.include <bsd.prog.mk>
