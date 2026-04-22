# $FreeBSD: releng/11.0/lib/clang/libllvmx86disassembler/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <src.opts.mk>

LIB=	llvmx86disassembler

SRCDIR=	lib/Target/X86/Disassembler
INCDIR=	lib/Target/X86
SRCS=	X86Disassembler.cpp \
	X86DisassemblerDecoder.cpp

TGHDRS=	X86GenDisassemblerTables \
	X86GenInstrInfo \
	X86GenRegisterInfo \
	X86GenSubtargetInfo

.include "../clang.lib.mk"
