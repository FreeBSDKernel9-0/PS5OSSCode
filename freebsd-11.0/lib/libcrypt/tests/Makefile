# $FreeBSD: releng/11.0/lib/libcrypt/tests/Makefile 299094 2016-05-04 23:20:53Z ngie $

ATF_TESTS_C+= crypt_tests

NETBSD_ATF_TESTS_C+= crypt_test

CFLAGS+= -I${.CURDIR:H}
LIBADD=	crypt

.include <netbsd-tests.test.mk>

.include <bsd.test.mk>
