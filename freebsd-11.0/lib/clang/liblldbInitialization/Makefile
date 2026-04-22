# $FreeBSD: releng/11.0/lib/clang/liblldbInitialization/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

LIB=	lldbInitialization

SRCDIR=	tools/lldb/source/Initialization
SRCS=	SystemInitializer.cpp \
	SystemInitializerCommon.cpp \
	SystemLifetimeManager.cpp
	
TGHDRS=	CommentCommandList \
	DeclNodes \
	DiagnosticCommonKinds \
	StmtNodes

.include "../lldb.lib.mk"
