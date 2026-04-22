# $FreeBSD: releng/11.0/lib/clang/libclangserialization/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

LIB=	clangserialization

SRCDIR=	tools/clang/lib/Serialization
SRCS=	ASTCommon.cpp \
	ASTReader.cpp \
	ASTReaderDecl.cpp \
	ASTReaderStmt.cpp \
	ASTWriter.cpp \
	ASTWriterDecl.cpp \
	ASTWriterStmt.cpp \
	GeneratePCH.cpp \
	GlobalModuleIndex.cpp \
	Module.cpp \
	ModuleFileExtension.cpp \
	ModuleManager.cpp

TGHDRS=	AttrList \
	AttrPCHRead \
	AttrPCHWrite \
	AttrParsedAttrList \
	Attrs \
	CommentCommandList \
	CommentNodes \
	DeclNodes \
	DiagnosticCommonKinds \
	DiagnosticFrontendKinds \
	DiagnosticSemaKinds \
	DiagnosticSerializationKinds \
	StmtNodes

.include "../clang.lib.mk"
