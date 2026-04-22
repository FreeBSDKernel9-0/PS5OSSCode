# $FreeBSD: releng/11.0/lib/clang/libllvmtarget/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <src.opts.mk>

LIB=	llvmtarget

SRCDIR=	lib/Target
SRCS=	Target.cpp \
	TargetLoweringObjectFile.cpp \
	TargetMachine.cpp \
	TargetMachineC.cpp \
	TargetRecip.cpp \
	TargetSubtargetInfo.cpp

.if ${MK_CLANG_EXTRAS} != "no"
SRCS+=	TargetIntrinsicInfo.cpp
.endif
	
TGHDRS=	Attributes \
	Intrinsics

.include "../clang.lib.mk"
