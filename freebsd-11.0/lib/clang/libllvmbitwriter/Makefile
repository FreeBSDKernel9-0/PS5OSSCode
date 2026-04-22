# $FreeBSD: releng/11.0/lib/clang/libllvmbitwriter/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <src.opts.mk>

LIB=	llvmbitwriter

SRCDIR=	lib/Bitcode/Writer
SRCS=	BitcodeWriter.cpp \
	BitcodeWriterPass.cpp \
	ValueEnumerator.cpp

.if ${MK_CLANG_EXTRAS} != "no"
SRCS+=	BitWriter.cpp
.endif

TGHDRS=	Attributes \
	Intrinsics

.include "../clang.lib.mk"
