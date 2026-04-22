# $FreeBSD: releng/11.0/usr.bin/mkuzip/Makefile 298504 2016-04-23 07:23:43Z sobomax $

PROG=	mkuzip
MAN=	mkuzip.8
SRCS=	mkuzip.c mkuz_blockcache.c mkuz_lzma.c mkuz_zlib.c mkuz_conveyor.c \
	    mkuz_blk.c mkuz_fqueue.c mkuz_time.c

#CFLAGS+=	-DMKUZ_DEBUG

LIBADD=	z md lzma pthread

.include <bsd.prog.mk>
