# $FreeBSD: releng/11.0/usr.sbin/ntp/doc/hints/Makefile 280849 2015-03-30 13:30:15Z cy $

.include <src.opts.mk>

FILESDIR=	${SHAREDIR}/doc/ntp/hints

.if ${MK_HTML} != "no"
FILES=	a-ux aix bsdi changes decosf1 decosf2 freebsd hpux linux mpeix \
	notes-xntp-v3 parse refclocks rs6000 sco.html sgi \
	solaris-dosynctodr.html solaris.html solaris.xtra.4023118 \
	solaris.xtra.4095849 solaris.xtra.S99ntpd solaris.xtra.patchfreq \
	sun4 svr4-dell svr4_package todo vxworks.html winnt.html
.endif

.PATH: ${.CURDIR}/../../../../contrib/ntp/html/hints

.include <bsd.prog.mk>
