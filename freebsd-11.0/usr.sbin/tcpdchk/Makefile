# $FreeBSD: releng/11.0/usr.sbin/tcpdchk/Makefile 291348 2015-11-26 01:14:40Z bdrewery $

.include <src.opts.mk>

.PATH: ${.CURDIR}/../../contrib/tcp_wrappers

PROG=	tcpdchk
MAN=	tcpdchk.8
SRCS=	tcpdchk.c fakelog.c inetcf.c scaffold.c

CFLAGS+=-DREAL_DAEMON_DIR=\"${LIBEXECDIR}\" \
	-DSEVERITY=LOG_INFO -DRFC931_TIMEOUT=10 -DPROCESS_OPTIONS \
	-DHOSTS_DENY=\"/etc/hosts.deny\" -DHOSTS_ALLOW=\"/etc/hosts.allow\"
.if ${MK_INET6_SUPPORT} != "no"
CFLAGS+=-DINET6
.endif

WARNS?=	0

LIBADD=	wrap

.include <bsd.prog.mk>
