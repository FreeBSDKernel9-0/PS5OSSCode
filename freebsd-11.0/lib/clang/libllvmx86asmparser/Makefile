# $FreeBSD: releng/11.0/lib/clang/libllvmx86asmparser/Makefile 276479 2014-12-31 20:31:32Z dim $

.include <bsd.own.mk>

LIB=	llvmx86asmparser

SRCDIR=	lib/Target/X86/AsmParser
INCDIR=	lib/Target/X86
SRCS=	X86AsmInstrumentation.cpp \
	X86AsmParser.cpp

TGHDRS=	X86GenAsmMatcher \
	X86GenInstrInfo \
	X86GenRegisterInfo \
	X86GenSubtargetInfo

.include "../clang.lib.mk"
