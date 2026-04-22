# $FreeBSD: releng/11.0/lib/clang/libllvmaarch64asmparser/Makefile 276783 2015-01-07 19:06:27Z dim $

.include <bsd.own.mk>

LIB=	llvmaarch64asmparser

SRCDIR=	lib/Target/AArch64/AsmParser
INCDIR=	lib/Target/AArch64
SRCS=	AArch64AsmParser.cpp

TGHDRS=	AArch64GenAsmMatcher \
	AArch64GenInstrInfo \
	AArch64GenRegisterInfo \
	AArch64GenSubtargetInfo

.include "../clang.lib.mk"
