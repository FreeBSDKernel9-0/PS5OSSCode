# $FreeBSD: releng/11.0/sbin/quotacheck/Makefile 298107 2016-04-16 07:45:30Z gjb $
#	@(#)Makefile	8.1 (Berkeley) 6/5/93

PACKAGE=quotacheck
PROG=	quotacheck
SRCS=	quotacheck.c preen.c fsutil.c utilities.c
WARNS?=	2
MAN=	quotacheck.8
LIBADD=	util

.PATH:	${.CURDIR}/../fsck ${.CURDIR}/../fsck_ffs

.include <bsd.prog.mk>
