# $FreeBSD: releng/11.0/lib/clang/libllvmarminfo/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

LIB=	llvmarminfo

SRCDIR=	lib/Target/ARM/TargetInfo
INCDIR=	lib/Target/ARM
SRCS=	ARMTargetInfo.cpp

TGHDRS=	ARMGenInstrInfo \
	ARMGenRegisterInfo \
	ARMGenSubtargetInfo \
	Attributes

.include "../clang.lib.mk"
