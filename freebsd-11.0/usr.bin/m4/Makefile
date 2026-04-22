#	$OpenBSD: Makefile,v 1.13 2014/05/12 19:11:19 espie Exp $
# $FreeBSD: releng/11.0/usr.bin/m4/Makefile 290351 2015-11-04 07:20:55Z rodrigc $

# -DEXTENDED 
# 	if you want the paste & spaste macros.

.include <src.opts.mk>

PROG=	m4
CFLAGS+=-DEXTENDED -I${.CURDIR} -I${.CURDIR}/../../lib/libopenbsd
LIBADD=	y l m openbsd

NO_WMISSING_VARIABLE_DECLARATIONS=

SRCS=	eval.c expr.c look.c main.c misc.c gnum4.c trace.c parser.y tokenizer.l

WARNS=	3

tokenizer.o: parser.h

CLEANFILES+=	parser.c parser.h tokenizer.o

.if ${MK_TESTS} != "no"
SUBDIR+= tests
.endif

.include <bsd.prog.mk>
