#	@(#)Makefile	8.1 (Berkeley) 6/6/93
# $FreeBSD: releng/11.0/usr.sbin/syslogd/Makefile 275054 2014-11-25 16:57:27Z bapt $

.include <src.opts.mk>

.PATH: ${.CURDIR}/../../usr.bin/wall

PROG=	syslogd
MAN=	syslog.conf.5 syslogd.8
SRCS=	syslogd.c ttymsg.c

LIBADD=	util

WARNS?= 3

.if ${MK_INET6_SUPPORT} != "no"
CFLAGS+= -DINET6
.endif

CFLAGS+= -I${.CURDIR}/../../usr.bin/wall

.include <bsd.prog.mk>
