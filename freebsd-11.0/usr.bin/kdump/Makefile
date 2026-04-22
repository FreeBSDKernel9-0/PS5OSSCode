#	@(#)Makefile	8.1 (Berkeley) 6/6/93
# $FreeBSD: releng/11.0/usr.bin/kdump/Makefile 296047 2016-02-25 18:23:40Z oshogbo $

.include <src.opts.mk>

.PATH: ${.CURDIR}/../ktrace

PROG=		kdump
SRCS=		kdump_subr.c kdump_subr.h kdump.c subr.c
CFLAGS+=	-I${.CURDIR}/../ktrace -I${.CURDIR} -I${.CURDIR}/../.. -I.

LIBADD=		sysdecode
.if ${MK_CASPER} != "no"
LIBADD+=	casper
LIBADD+=	cap_grp
LIBADD+=	cap_pwd
CFLAGS+=-DHAVE_LIBCASPER
.endif

NO_WERROR?=	YES

CLEANFILES=	kdump_subr.c kdump_subr.h

kdump_subr.h: mksubr
	sh ${.CURDIR}/mksubr ${DESTDIR}${INCLUDEDIR} | \
	    sed -n 's/^\([a-z].*)\)$$/void \1;/p' >${.TARGET}

kdump_subr.c: mksubr kdump_subr.h
	sh ${.CURDIR}/mksubr ${DESTDIR}${INCLUDEDIR} >${.TARGET}

.include <bsd.prog.mk>
