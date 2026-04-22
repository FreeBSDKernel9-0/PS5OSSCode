# $FreeBSD: releng/11.0/lib/clang/libclangstaticanalyzerfrontend/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

LIB=	clangstaticanalyzerfrontend

SRCDIR=	tools/clang/lib/StaticAnalyzer/Frontend
SRCS=	AnalysisConsumer.cpp \
	CheckerRegistration.cpp \
	FrontendActions.cpp \
	ModelConsumer.cpp \
	ModelInjector.cpp

TGHDRS=	AttrList \
	AttrParsedAttrList \
	AttrVisitor \
	Attrs \
	Checkers \
	CommentCommandList \
	CommentNodes \
	DeclNodes \
	DiagnosticCommonKinds \
	DiagnosticFrontendKinds \
	StmtNodes

.include "../clang.lib.mk"
