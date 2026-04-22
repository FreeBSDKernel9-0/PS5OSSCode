# $FreeBSD: releng/11.0/tests/sys/kqueue/Makefile 299094 2016-05-04 23:20:53Z ngie $
#
# svn://mark.heily.com/libkqueue/trunk/test
# Last update: r114
#
# libkqueue and test suite by Mark Heily <mark@heily.com>
#

TAP_TESTS_SH=	kqueue_test

TESTSDIR=	${TESTSBASE}/sys/kqueue
BINDIR=		${TESTSDIR}

PROGS=	kqtest

SRCS.kqtest=		\
	main.c		\
	read.c		\
	timer.c		\
	vnode.c		\
	proc.c		\
	signal.c	\
	user.c
WARNS?=	2

.include <bsd.test.mk>
