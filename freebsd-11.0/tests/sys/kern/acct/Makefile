# $FreeBSD: releng/11.0/tests/sys/kern/acct/Makefile 299094 2016-05-04 23:20:53Z ngie $

TESTSDIR=	${TESTSBASE}/sys/kern/acct

ATF_TESTS_C=	acct_test

CFLAGS+=	-I${.OBJDIR}

CLEANFILES+=	convert.c convert.c.tmp

DPSRCS.acct_test=	convert.c
acct_test.o: convert.c

convert.c: ${SRCTOP}/sys/kern/kern_acct.c
	sed -n -e 's/log(/syslog(/g' \
	       -e '/FLOAT_CONVERSION_START/,/FLOAT_CONVERSION_END/p' ${.ALLSRC} >${.TARGET}.tmp
	mv ${.TARGET}.tmp ${.TARGET}

.include <bsd.test.mk>
