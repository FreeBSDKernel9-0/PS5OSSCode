# $FreeBSD: releng/11.0/etc/periodic/weekly/Makefile 265420 2014-05-06 04:22:01Z imp $

.include <src.opts.mk>

FILES=	340.noid \
	450.status-security \
	999.local

# NB: keep these sorted by MK_* knobs

.if ${MK_LOCATE} != "no"
FILES+=	310.locate
.endif

.if ${MK_MAN_UTILS} != "no"
FILES+=	320.whatis 330.catman
.endif

.include <bsd.prog.mk>
