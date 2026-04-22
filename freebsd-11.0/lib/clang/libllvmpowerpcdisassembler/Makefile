# $FreeBSD: releng/11.0/lib/clang/libllvmpowerpcdisassembler/Makefile 274973 2014-11-24 19:46:35Z dim $

.include <bsd.own.mk>

LIB=	llvmpowerpcdisassembler

SRCDIR=	lib/Target/PowerPC/Disassembler
INCDIR=	lib/Target/PowerPC
SRCS=	PPCDisassembler.cpp

TGHDRS=	PPCGenDisassemblerTables \
	PPCGenInstrInfo \
	PPCGenRegisterInfo \
	PPCGenSubtargetInfo

.include "../clang.lib.mk"
