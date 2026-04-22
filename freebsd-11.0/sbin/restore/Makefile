#	@(#)Makefile	8.1 (Berkeley) 6/5/93
# $FreeBSD: releng/11.0/sbin/restore/Makefile 298107 2016-04-16 07:45:30Z gjb $

.PATH: ${.CURDIR}/../dump

PACKAGE=runtime
PROG=	restore
LINKS=	${BINDIR}/restore ${BINDIR}/rrestore
MAN=	restore.8
MLINKS=	restore.8 rrestore.8
SRCS=	main.c interactive.c restore.c dirs.c symtab.c tape.c utilities.c \
	dumprmt.c
WARNS?=	2
CFLAGS+= -DRRESTORE -D_ACL_PRIVATE

.include <bsd.prog.mk>
