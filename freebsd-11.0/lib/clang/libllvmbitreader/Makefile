# $FreeBSD: releng/11.0/lib/clang/libllvmbitreader/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <src.opts.mk>

LIB=	llvmbitreader

SRCDIR=	lib/Bitcode/Reader
SRCS=	BitcodeReader.cpp \
	BitstreamReader.cpp

.if ${MK_CLANG_EXTRAS} != "no"
SRCS+=	BitReader.cpp
.endif

TGHDRS=	Attributes \
	Intrinsics

.include "../clang.lib.mk"
