# $FreeBSD: releng/11.0/usr.sbin/fifolog/fifolog_reader/Makefile 281384 2015-04-10 21:24:38Z emaste $

PROG=	fifolog_reader

CFLAGS+= -I${.CURDIR}/../lib

MAN=

LIBADD=	fifolog

regress:
	./${PROG} /tmp/fifolog.0
	./${PROG} -t /tmp/fifolog.0
	./${PROG} /tmp/fifolog.1
	./${PROG} -B "00:00" /tmp/fifolog.1
	./${PROG} -T "%y%m%d-%H%M%S" /tmp/fifolog.1
	./${PROG} -T "" /tmp/fifolog.1

.include <bsd.prog.mk>
