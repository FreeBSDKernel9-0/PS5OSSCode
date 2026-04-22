# $FreeBSD: releng/11.0/lib/clang/liblldbDataFormatters/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

LIB=	lldbDataFormatters

SRCDIR=	tools/lldb/source/DataFormatters
SRCS=	CXXFunctionPointer.cpp \
	DataVisualization.cpp \
	DumpValueObjectOptions.cpp \
	FormatCache.cpp \
	FormatClasses.cpp \
	FormatManager.cpp \
	FormattersHelpers.cpp \
	LanguageCategory.cpp \
	StringPrinter.cpp \
	TypeCategory.cpp \
	TypeCategoryMap.cpp \
	TypeFormat.cpp \
	TypeSummary.cpp \
	TypeSynthetic.cpp \
	TypeValidator.cpp \
	ValueObjectPrinter.cpp \
	VectorType.cpp

TGHDRS=	AttrList \
	Attrs \
	DiagnosticCommonKinds \
	DeclNodes \
	StmtNodes \
	CommentCommandList

.include "../lldb.lib.mk"
