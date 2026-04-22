# $FreeBSD: releng/11.0/etc/periodic/monthly/Makefile 298107 2016-04-16 07:45:30Z gjb $

.include <src.opts.mk>

FILESGROUPS=FILES

FILES=	450.status-security \
	999.local

# NB: keep these sorted by MK_* knobs

.if ${MK_UTMPX} != "no"
FILESGROUPS+=	ACCT
ACCT+=		200.accounting
.endif
ACCTDIR=	/etc/periodic/monthly
ACCTMODE=	${BINMODE}
ACCTPACKAGE=	acct

.include <bsd.prog.mk>
