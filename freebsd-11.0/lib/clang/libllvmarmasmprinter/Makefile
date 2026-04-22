# $FreeBSD: releng/11.0/lib/clang/libllvmarmasmprinter/Makefile 292988 2015-12-31 14:39:45Z dim $

.include <bsd.own.mk>

LIB=	llvmarmasmprinter

SRCDIR=	lib/Target/ARM/InstPrinter
INCDIR=	lib/Target/ARM
SRCS=	ARMInstPrinter.cpp

TGHDRS=	ARMGenAsmWriter \
	ARMGenInstrInfo \
	ARMGenRegisterInfo \
	ARMGenSubtargetInfo

.include "../clang.lib.mk"
