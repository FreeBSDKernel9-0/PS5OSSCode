# $FreeBSD: releng/11.0/lib/clang/libllvmaarch64disassembler/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

LIB=	llvmaarch64disassembler

SRCDIR=	lib/Target/AArch64/Disassembler
INCDIR=	lib/Target/AArch64
SRCS=	AArch64Disassembler.cpp \
	AArch64ExternalSymbolizer.cpp

TGHDRS=	Attributes \
	Intrinsics \
	AArch64GenDisassemblerTables \
	AArch64GenInstrInfo \
	AArch64GenRegisterInfo \
	AArch64GenSubtargetInfo

.include "../clang.lib.mk"
