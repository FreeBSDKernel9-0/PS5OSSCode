# $FreeBSD: releng/11.0/lib/clang/libllvmsparcasmparser/Makefile 276479 2014-12-31 20:31:32Z dim $

.include <bsd.own.mk>

LIB=	llvmsparcasmparser

SRCDIR=	lib/Target/Sparc/AsmParser
INCDIR=	lib/Target/Sparc
SRCS=	SparcAsmParser.cpp

TGHDRS=	SparcGenAsmMatcher \
	SparcGenInstrInfo \
	SparcGenRegisterInfo \
	SparcGenSubtargetInfo

.include "../clang.lib.mk"
