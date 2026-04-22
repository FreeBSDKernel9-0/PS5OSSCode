# $FreeBSD: releng/11.0/etc/newsyslog.conf.d/Makefile 277541 2015-01-22 23:50:47Z ngie $

.include <src.opts.mk>

BINDIR=		/etc/newsyslog.conf.d

FILES=

.if ${MK_OFED} != "no"
FILES+=		opensm.conf
.endif

.include <bsd.prog.mk>
