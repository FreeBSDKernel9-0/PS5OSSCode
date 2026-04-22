# $FreeBSD: releng/11.0/sys/boot/i386/mbr/Makefile 264400 2014-04-13 05:21:56Z imp $

PROG=	mbr
STRIP=
BINMODE=${NOBINMODE}
MAN=
SRCS=	${PROG}.s

# MBR flags: 0x80 -- try packet interface (also known as EDD or LBA)
BOOT_MBR_FLAGS?=	0x80

ORG=	0x600

AFLAGS+=--defsym FLAGS=${BOOT_MBR_FLAGS}
LDFLAGS=-e start -Ttext ${ORG} -Wl,-N,-S,--oformat,binary

.include <bsd.prog.mk>
