# $FreeBSD: releng/11.0/lib/clang/libllvmaarch64desc/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

LIB=	llvmaarch64desc

SRCDIR=	lib/Target/AArch64/MCTargetDesc
SRCS=	AArch64AsmBackend.cpp \
	AArch64ELFObjectWriter.cpp \
	AArch64ELFStreamer.cpp \
	AArch64MCAsmInfo.cpp \
	AArch64MCCodeEmitter.cpp \
	AArch64MCExpr.cpp \
	AArch64MCTargetDesc.cpp \
	AArch64MachObjectWriter.cpp \
	AArch64TargetStreamer.cpp
CFLAGS+= -I${LLVM_SRCS}/${SRCDIR}/..

TGHDRS=	AArch64GenInstrInfo \
	AArch64GenMCCodeEmitter \
	AArch64GenRegisterInfo \
	AArch64GenSubtargetInfo \
	Attributes

.include "../clang.lib.mk"
