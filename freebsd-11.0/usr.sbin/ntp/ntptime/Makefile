# $FreeBSD: releng/11.0/usr.sbin/ntp/ntptime/Makefile 280849 2015-03-30 13:30:15Z cy $

.PATH: ${.CURDIR}/../../../contrib/ntp/util

PROG=	ntptime
MAN=

CFLAGS+= -I${.CURDIR}/../../../contrib/ntp/include \
	-I${.CURDIR}/../../../contrib/ntp/lib/isc/include/ \
	-I${.CURDIR}/../../../contrib/ntp/lib/isc/unix/include/ \
	-I${.CURDIR}/../../../contrib/ntp/lib/isc/pthreads/include \
	-I${.CURDIR}/../

LIBADD=	ntp pthread

.include <bsd.prog.mk>
