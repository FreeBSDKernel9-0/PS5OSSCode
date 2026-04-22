# $FreeBSD: releng/11.0/lib/libutil/tests/Makefile 299094 2016-05-04 23:20:53Z ngie $

TAP_TESTS_C+=	flopen_test
TAP_TESTS_C+=	grp_test
TAP_TESTS_C+=	humanize_number_test
.if ${MACHINE_CPUARCH} != "aarch64" # PR202304: pidfile_test hangs on arm64
TAP_TESTS_C+=	pidfile_test
.endif
TAP_TESTS_C+=	trimdomain_test
TAP_TESTS_C+=	trimdomain-nodomain_test

LIBADD+=	util

.include <bsd.test.mk>
