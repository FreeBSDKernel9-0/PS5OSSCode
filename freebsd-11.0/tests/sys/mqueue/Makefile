# $FreeBSD: releng/11.0/tests/sys/mqueue/Makefile 299094 2016-05-04 23:20:53Z ngie $

TESTSDIR=	${TESTSBASE}/sys/mqueue

ATF_TESTS_SH=	mqueue_test

BINDIR=		${TESTSDIR}

CFLAGS+=	-I${SRCTOP}/tests

PROGS+=		mqtest1
PROGS+=		mqtest2
#PROGS+=		mqtest3
#PROGS+=		mqtest4
PROGS+=		mqtest5

LIBADD+=	rt

WARNS?=		6

.include <bsd.test.mk>
