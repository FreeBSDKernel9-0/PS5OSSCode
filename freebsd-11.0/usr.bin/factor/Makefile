#	@(#)Makefile	8.1 (Berkeley) 5/31/93
# $FreeBSD: releng/11.0/usr.bin/factor/Makefile 291738 2015-12-04 03:17:47Z bdrewery $

.include <src.opts.mk>

PROG=	factor
SRCS=	factor.c pr_tbl.c
CFLAGS+=-I${.CURDIR}/../primes

.if ${MK_OPENSSL} != "no"
CFLAGS+=-DHAVE_OPENSSL
LIBADD+=crypto
.endif

MAN=	factor.6
MLINKS+=factor.6 primes.6
.PATH:	${.CURDIR}/../primes

.include <bsd.prog.mk>
