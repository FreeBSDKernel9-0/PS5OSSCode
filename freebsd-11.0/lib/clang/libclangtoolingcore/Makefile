# $FreeBSD: releng/11.0/lib/clang/libclangtoolingcore/Makefile 301993 2016-06-17 16:46:58Z bdrewery $

.include <bsd.own.mk>

LIB=	clangtoolingcore

SRCDIR=	tools/clang/lib/Tooling/Core
CFLAGS+= -I${LLVM_SRCS}/tools
SRCS=	Lookup.cpp \
	Replacement.cpp

TGHDRS=	AttrList \
	Attrs \
	CommentCommandList \
	DeclNodes \
	DiagnosticCommonKinds \
	StmtNodes

.include "../clang.lib.mk"
