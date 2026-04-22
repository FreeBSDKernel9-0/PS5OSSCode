# $FreeBSD: releng/11.0/lib/libbz2/Makefile 298107 2016-04-16 07:45:30Z gjb $

PACKAGE=	lib${LIB}
BZ2DIR=	${.CURDIR}/../../contrib/bzip2
.PATH: ${BZ2DIR}

LIB=		bz2
SHLIB_MAJOR=	4
SRCS=		bzlib.c blocksort.c compress.c crctable.c decompress.c \
		huffman.c randtable.c
INCS=		bzlib.h
CFLAGS+=	-I${BZ2DIR}

WARNS?=		3

.include <bsd.lib.mk>
