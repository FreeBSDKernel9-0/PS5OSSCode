# $FreeBSD: releng/11.0/sbin/dhclient/tests/Makefile 299094 2016-05-04 23:20:53Z ngie $

.PATH:	${.CURDIR:H}

PLAIN_TESTS_C=				option-domain-search_test
SRCS.option-domain-search_test=		alloc.c convert.c hash.c options.c \
					tables.c fake.c option-domain-search.c
CFLAGS.option-domain-search_test+=	-I${.CURDIR}/..
LIBADD.option-domain-search_test=	util

WARNS?=	2

.include <bsd.test.mk>
