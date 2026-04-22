# $FreeBSD: releng/11.0/lib/clang/libclangrewritefrontend/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <src.opts.mk>

LIB=	clangrewritefrontend

SRCDIR=	tools/clang/lib/Frontend/Rewrite
SRCS=	FixItRewriter.cpp \
	FrontendActions.cpp \
	HTMLPrint.cpp \
	InclusionRewriter.cpp \
	RewriteMacros.cpp \
	RewriteTest.cpp

.if ${MK_CLANG_EXTRAS} != "no"
SRCS+=	RewriteModernObjC.cpp \
	RewriteObjC.cpp
.endif

TGHDRS=	AttrList \
	AttrParsedAttrList \
	Attrs \
	CommentCommandList \
	DeclNodes \
	DiagnosticCommonKinds \
	DiagnosticFrontendKinds \
	StmtNodes

.include "../clang.lib.mk"
