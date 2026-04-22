# $FreeBSD: releng/11.0/lib/clang/libllvmmipsasmparser/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

LIB=	llvmmipsasmparser

SRCDIR=	lib/Target/Mips/AsmParser
INCDIR=	lib/Target/Mips
SRCS=	MipsAsmParser.cpp

TGHDRS=	Attributes \
	MipsGenAsmMatcher \
	MipsGenInstrInfo \
	MipsGenRegisterInfo \
	MipsGenSubtargetInfo

.include "../clang.lib.mk"
