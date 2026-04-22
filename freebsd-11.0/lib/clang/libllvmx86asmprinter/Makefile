# $FreeBSD: releng/11.0/lib/clang/libllvmx86asmprinter/Makefile 292988 2015-12-31 14:39:45Z dim $

.include <bsd.own.mk>

LIB=	llvmx86asmprinter

SRCDIR=	lib/Target/X86/InstPrinter
INCDIR=	lib/Target/X86
SRCS=	X86ATTInstPrinter.cpp \
	X86InstComments.cpp \
	X86IntelInstPrinter.cpp

TGHDRS=	X86GenAsmWriter \
	X86GenAsmWriter1 \
	X86GenInstrInfo \
	X86GenRegisterInfo \
	X86GenSubtargetInfo

.include "../clang.lib.mk"
