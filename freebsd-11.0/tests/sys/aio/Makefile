# $FreeBSD: releng/11.0/tests/sys/aio/Makefile 299094 2016-05-04 23:20:53Z ngie $

TESTSDIR=	${TESTSBASE}/sys/aio

ATF_TESTS_C+=	aio_test

PLAIN_TESTS_C+=	aio_kqueue_test
PLAIN_TESTS_C+=	lio_kqueue_test

LIBADD.aio_test+=	util

CFLAGS+=	-I${.CURDIR:H:H}

WARNS?=	6

.include <bsd.test.mk>
