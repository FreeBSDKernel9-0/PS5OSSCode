#	@(#)Makefile	8.1 (Berkeley) 6/6/93
# $FreeBSD: releng/11.0/usr.bin/w/Makefile 287590 2015-09-09 05:17:04Z delphij $

PROG=	w
SRCS=	fmt.c pr_time.c proc_compare.c w.c
MAN=	w.1 uptime.1
LIBADD=	kvm sbuf xo util
LINKS=	${BINDIR}/w ${BINDIR}/uptime

.PATH: ${.CURDIR}/../../bin/ps

.include <bsd.prog.mk>
