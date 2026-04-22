# $FreeBSD: releng/11.0/lib/clang/libllvmobject/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <src.opts.mk>

LIB=	llvmobject

SRCDIR=	lib/Object
SRCS=	Archive.cpp \
	ArchiveWriter.cpp \
	Binary.cpp \
	COFFObjectFile.cpp \
	COFFYAML.cpp \
	ELF.cpp \
	ELFObjectFile.cpp \
	ELFYAML.cpp \
	Error.cpp \
	FunctionIndexObjectFile.cpp \
	IRObjectFile.cpp \
	MachOObjectFile.cpp \
	MachOUniversal.cpp \
	ObjectFile.cpp \
	RecordStreamer.cpp \
	SymbolicFile.cpp

.if ${MK_CLANG_EXTRAS} != "no"
SRCS+=	Object.cpp \
	SymbolSize.cpp
.endif

TGHDRS=	Attributes
	
.include "../clang.lib.mk"
