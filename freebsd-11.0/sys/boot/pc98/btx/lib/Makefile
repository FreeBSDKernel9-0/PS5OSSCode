# $FreeBSD: releng/11.0/sys/boot/pc98/btx/lib/Makefile 270168 2014-08-19 15:04:32Z bdrewery $

PROG=	crt0.o
INTERNALPROG=
MAN=
SRCS=	btxcsu.S btxsys.s btxv86.s
CFLAGS+=-I${.CURDIR}/../../../i386/common
LDFLAGS=-Wl,-r

.include <bsd.prog.mk>
