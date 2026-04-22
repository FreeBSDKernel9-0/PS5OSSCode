# $FreeBSD: releng/11.0/usr.sbin/sa/Makefile 298107 2016-04-16 07:45:30Z gjb $

.include <src.opts.mk>

.PATH: ${.CURDIR}/../../usr.bin/lastcomm

PROG=	sa
MAN=	sa.8
SRCS=	main.c db.c pdb.c usrdb.c readrec.c

PACKAGE=acct

.if ${MK_TESTS} != "no"
SUBDIR+= tests
.endif

.include <bsd.prog.mk>
