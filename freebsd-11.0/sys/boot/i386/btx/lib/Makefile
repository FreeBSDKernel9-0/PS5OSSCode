# $FreeBSD: releng/11.0/sys/boot/i386/btx/lib/Makefile 270168 2014-08-19 15:04:32Z bdrewery $

PROG=	crt0.o
INTERNALPROG=
MAN=
SRCS=	btxcsu.S btxsys.s btxv86.s
CFLAGS+=-I${.CURDIR}/../../common
LDFLAGS=-Wl,-r

.include <bsd.prog.mk>
