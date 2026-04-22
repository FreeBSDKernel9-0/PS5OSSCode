# $FreeBSD: releng/11.0/lib/clang/libllvmpowerpcasmprinter/Makefile 292988 2015-12-31 14:39:45Z dim $

.include <bsd.own.mk>

LIB=	llvmpowerpcasmprinter

SRCDIR=	lib/Target/PowerPC/InstPrinter
INCDIR=	lib/Target/PowerPC
SRCS=	PPCInstPrinter.cpp

TGHDRS=	PPCGenAsmWriter \
	PPCGenInstrInfo \
	PPCGenRegisterInfo \
	PPCGenSubtargetInfo

.include "../clang.lib.mk"
