# $FreeBSD: releng/11.0/lib/clang/libllvmmipsdesc/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

LIB=	llvmmipsdesc

SRCDIR=	lib/Target/Mips/MCTargetDesc
SRCS=	MipsABIFlagsSection.cpp \
	MipsABIInfo.cpp \
	MipsAsmBackend.cpp \
	MipsELFObjectWriter.cpp \
	MipsELFStreamer.cpp \
	MipsMCAsmInfo.cpp \
	MipsMCCodeEmitter.cpp \
	MipsMCExpr.cpp \
	MipsMCTargetDesc.cpp \
	MipsNaClELFStreamer.cpp \
	MipsOptionRecord.cpp \
	MipsTargetStreamer.cpp
CFLAGS+= -I${LLVM_SRCS}/${SRCDIR}/..

TGHDRS=	Attributes \
	MipsGenInstrInfo \
	MipsGenMCCodeEmitter \
	MipsGenRegisterInfo \
	MipsGenSubtargetInfo

.include "../clang.lib.mk"
