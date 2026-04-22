# $FreeBSD: releng/11.0/lib/clang/liblldbHostFreeBSD/Makefile 280031 2015-03-15 13:31:13Z dim $

.include <bsd.own.mk>

LIB=	lldbHostFreeBSD

SRCDIR=	tools/lldb/source/Host/freebsd
SRCS=	Host.cpp \
	HostInfoFreeBSD.cpp \
	HostThreadFreeBSD.cpp \
	ThisThread.cpp

TGHDRS=	DiagnosticCommonKinds \
	DeclNodes \
	StmtNodes \
	CommentCommandList

.include "../lldb.lib.mk"
