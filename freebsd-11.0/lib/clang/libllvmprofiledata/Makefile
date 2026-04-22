# $FreeBSD: releng/11.0/lib/clang/libllvmprofiledata/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <src.opts.mk>

LIB=	llvmprofiledata

SRCDIR=	lib/ProfileData
SRCS=	CoverageMapping.cpp \
	CoverageMappingReader.cpp \
	CoverageMappingWriter.cpp \
	InstrProf.cpp \
	InstrProfReader.cpp \
	InstrProfWriter.cpp \
	SampleProf.cpp \
	SampleProfReader.cpp

.if ${MK_CLANG_EXTRAS} != "no"
SRCS+=	SampleProfWriter.cpp
.endif

TGHDRS=	Attributes

.include "../clang.lib.mk"
