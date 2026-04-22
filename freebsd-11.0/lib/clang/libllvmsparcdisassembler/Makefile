# $FreeBSD: releng/11.0/lib/clang/libllvmsparcdisassembler/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

LIB=	llvmsparcdisassembler

SRCDIR=	lib/Target/Sparc/Disassembler
INCDIR=	lib/Target/Sparc
SRCS=	SparcDisassembler.cpp

TGHDRS=	Attributes \
	Intrinsics \
	SparcGenDisassemblerTables \
	SparcGenInstrInfo \
	SparcGenRegisterInfo \
	SparcGenSubtargetInfo

.include "../clang.lib.mk"
