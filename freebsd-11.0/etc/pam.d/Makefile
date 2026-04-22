# $FreeBSD: releng/11.0/etc/pam.d/Makefile 298107 2016-04-16 07:45:30Z gjb $

.include <src.opts.mk>

NO_OBJ=

FILESGROUPS=	FILES

FILES=	README \
	cron \
	ftpd \
	imap \
	login \
	other \
	passwd pop3 \
	sshd su system \
	telnetd \
	xdm

.if ${MK_AT} != "no"
FILESGROUPS+=	AT
AT+=	atrun
ATPACKAGE+=	at
ATDIR=	/etc/pam.d
ATMODE=	644
.endif

.if ${MK_RCMDS} != "no"
FILESGROUPS+=	RCMDS
RCMDS+=		rsh
RCMDSPACKAGE+=	rcmds
RCMDSDIR=	/etc/pam.d
RCMDSMODE=	644
.endif

FILESDIR=	/etc/pam.d
FILESMODE=	644
FILESMODE_README=	444
LINKS=	${FILESDIR}/ftpd ${FILESDIR}/ftp

.include <bsd.prog.mk>
