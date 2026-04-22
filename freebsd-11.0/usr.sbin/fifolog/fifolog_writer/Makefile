# $FreeBSD: releng/11.0/usr.sbin/fifolog/fifolog_writer/Makefile 281384 2015-04-10 21:24:38Z emaste $

PROG=	fifolog_writer

CFLAGS+= -I${.CURDIR}/../lib

MAN=

LIBADD=	fifolog

regress:
	date | ./${PROG} -z 0 /tmp/fifolog.0
	lptest 65 | ./${PROG} -z 9 /tmp/fifolog.1

.include <bsd.prog.mk>
