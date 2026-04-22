# $FreeBSD: releng/11.0/sys/boot/i386/pmbr/Makefile 264400 2014-04-13 05:21:56Z imp $

PROG=	pmbr
STRIP=
BINMODE=${NOBINMODE}
MAN=
SRCS=	${PROG}.s

ORG=	0x600

AFLAGS+=--defsym FLAGS=${BOOT_MBR_FLAGS}
LDFLAGS=-e start -Ttext ${ORG} -Wl,-N,-S,--oformat,binary

.include <bsd.prog.mk>
