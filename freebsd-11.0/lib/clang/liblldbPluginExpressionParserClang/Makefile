# $FreeBSD: releng/11.0/lib/clang/liblldbPluginExpressionParserClang/Makefile 293124 2016-01-04 00:02:58Z emaste $

.include <bsd.own.mk>

LIB=	lldbPluginExpressionParserClang

SRCDIR=	tools/lldb/source/Plugins/ExpressionParser/Clang
SRCS=	ASTDumper.cpp \
	ASTResultSynthesizer.cpp \
	ASTStructExtractor.cpp \
	ClangASTSource.cpp \
	ClangExpressionDeclMap.cpp \
	ClangExpressionParser.cpp \
	ClangExpressionVariable.cpp \
	ClangFunctionCaller.cpp \
	ClangModulesDeclVendor.cpp \
	ClangPersistentVariables.cpp \
	ClangUserExpression.cpp \
	ClangUtilityFunction.cpp \
	IRForTarget.cpp

TGHDRS=	Attributes \
	AttrList \
	AttrParsedAttrList \
	Attrs \
	CommentCommandList \
	DeclNodes \
	DiagnosticCommonKinds \
	DiagnosticFrontendKinds \
	DiagnosticSemaKinds \
	Intrinsics \
	StmtNodes

.include "../lldb.lib.mk"
