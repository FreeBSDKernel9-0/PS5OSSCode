# $FreeBSD: releng/11.0/lib/clang/libllvmvectorize/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <src.opts.mk>

LIB=	llvmvectorize

SRCDIR=	lib/Transforms/Vectorize
SRCS=	BBVectorize.cpp \
	LoopVectorize.cpp \
	SLPVectorizer.cpp

.if ${MK_CLANG_EXTRAS} != "no"
SRCS+=	Vectorize.cpp
.endif

TGHDRS=	Attributes \
	Intrinsics

.include "../clang.lib.mk"
