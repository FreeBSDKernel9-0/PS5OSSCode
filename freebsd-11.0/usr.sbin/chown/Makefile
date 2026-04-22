#	@(#)Makefile	8.1 (Berkeley) 6/6/93
# $FreeBSD: releng/11.0/usr.sbin/chown/Makefile 268030 2014-06-30 05:33:52Z eadler $

.include <src.opts.mk>

PROG=	chown
LINKS=	${BINDIR}/chown /usr/bin/chgrp
MAN=	chgrp.1 chown.8

.if ${MK_TESTS} != "no"
SUBDIR+= tests
.endif

.include <bsd.prog.mk>
