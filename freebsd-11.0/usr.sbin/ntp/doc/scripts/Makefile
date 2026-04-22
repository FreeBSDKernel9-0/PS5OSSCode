# $FreeBSD: releng/11.0/usr.sbin/ntp/doc/scripts/Makefile 280849 2015-03-30 13:30:15Z cy $

.include <src.opts.mk>

FILESDIR=	${SHAREDIR}/doc/ntp/scripts

.if ${MK_HTML} != "no"
FILES=	accopt.txt audio.txt authopt.txt clockopt.txt command.txt config.txt \
	confopt.txt external.txt footer.txt hand.txt install.txt manual.txt \
	misc.txt miscopt.txt monopt.txt refclock.txt special.txt style.css
.endif

.PATH: ${.CURDIR}/../../../../contrib/ntp/html/scripts

.include <bsd.prog.mk>
