# $FreeBSD: releng/11.0/lib/clang/libllvmaarch64asmprinter/Makefile 292988 2015-12-31 14:39:45Z dim $

.include <bsd.own.mk>

LIB=	llvmaarch64asmprinter

SRCDIR=	lib/Target/AArch64/InstPrinter
INCDIR=	lib/Target/AArch64
SRCS=	AArch64InstPrinter.cpp

TGHDRS=	AArch64GenAsmWriter \
	AArch64GenAsmWriter1 \
	AArch64GenInstrInfo \
	AArch64GenRegisterInfo \
	AArch64GenSubtargetInfo

.include "../clang.lib.mk"
