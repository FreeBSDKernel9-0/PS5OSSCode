# $FreeBSD: releng/11.0/usr.sbin/bsnmpd/tools/Makefile.inc 299806 2016-05-15 00:08:09Z ngie $
# Author: Shteryana Shopova <syrinx@FreeBSD.org>

BINDIR?= /usr/bin
PACKAGE=	bsnmp

CFLAGS+= -I. -I${.CURDIR}

.if exists(${.OBJDIR}/../libbsnmptools)
LIBBSNMPTOOLSDIR=      ${.OBJDIR}/../libbsnmptools
.else
LIBBSNMPTOOLSDIR=      ${.CURDIR}/../libbsnmptools
.endif
LIBBSNMPTOOLS=         ${LIBBSNMPTOOLSDIR}/libbsnmptools.a

WARNS?=		6
