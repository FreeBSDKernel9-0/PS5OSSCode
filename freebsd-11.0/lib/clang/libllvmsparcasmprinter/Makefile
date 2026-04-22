# $FreeBSD: releng/11.0/lib/clang/libllvmsparcasmprinter/Makefile 292988 2015-12-31 14:39:45Z dim $

.include <bsd.own.mk>

LIB=	llvmsparcasmprinter

SRCDIR=	lib/Target/Sparc/InstPrinter
INCDIR=	lib/Target/Sparc
SRCS=	SparcInstPrinter.cpp

TGHDRS=	SparcGenAsmWriter \
	SparcGenInstrInfo \
	SparcGenRegisterInfo \
	SparcGenSubtargetInfo

.include "../clang.lib.mk"
