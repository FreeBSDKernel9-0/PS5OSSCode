# $FreeBSD: releng/11.0/usr.sbin/zic/zdump/Makefile 290083 2015-10-27 23:35:02Z bdrewery $

.PATH: ${.CURDIR}/../../../contrib/tzcode/zic

PROG=	zdump
MAN=	zdump.8
SRCS=	zdump.c ialloc.c scheck.c

CFLAGS+= -DTM_GMTOFF=tm_gmtoff -DTM_ZONE=tm_zone -DSTD_INSPIRED -DPCTS
CFLAGS+= -DHAVE_LONG_DOUBLE -DTZDIR=\"${SHAREDIR}/zoneinfo\" -Demkdir=mkdir
CFLAGS+= -I${.CURDIR}/.. -I${.CURDIR}/../../../contrib/tzcode/stdtime

WARNS?=	2

.include <bsd.prog.mk>
