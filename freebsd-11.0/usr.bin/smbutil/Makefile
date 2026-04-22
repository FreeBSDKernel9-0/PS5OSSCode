# $FreeBSD: releng/11.0/usr.bin/smbutil/Makefile 281333 2015-04-09 21:30:11Z bapt $

PROG=	smbutil
SRCS=	smbutil.c dumptree.c login.c lookup.c view.c print.c

LIBADD=	smb

CONTRIBDIR=	${.CURDIR}/../../contrib/smbfs
CFLAGS+=	-I${CONTRIBDIR}/include

WARNS?=	0

.PATH:	${CONTRIBDIR}/smbutil

.include <bsd.prog.mk>
