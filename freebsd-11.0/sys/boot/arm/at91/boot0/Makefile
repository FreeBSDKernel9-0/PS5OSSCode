#  $FreeBSD: releng/11.0/sys/boot/arm/at91/boot0/Makefile 264400 2014-04-13 05:21:56Z imp $

.PATH:	${.CURDIR}/../libat91

P=boot0
FILES=${P}
SRCS=arm_init.S main.c
MAN=
LDFLAGS=-e 0 -T ${.CURDIR}/linker.cfg
OBJS+=  ${SRCS:N*.h:R:S/$/.o/g}

.include <bsd.prog.mk>

CFLAGS+=-DBOOT_BOOT0
