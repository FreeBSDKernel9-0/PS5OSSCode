# $FreeBSD: releng/11.0/lib/clang/liblldbPluginProcessPOSIX/Makefile 288943 2015-10-06 17:53:29Z dim $

.include <bsd.own.mk>

LIB=	lldbPluginProcessPOSIX

# include_directories(.)
CFLAGS+=-I${.CURDIR}/../../../contrib/llvm/tools/lldb/source/Plugins/Process/POSIX
CFLAGS+=-I${.CURDIR}/../../../contrib/llvm/tools/lldb/source/Plugins/Process/Utility
CFLAGS+=-I${.CURDIR}/../../../contrib/llvm/tools/lldb/source/Plugins/Process/FreeBSD

SRCDIR=	tools/lldb/source/Plugins/Process/POSIX
SRCS=	CrashReason.cpp \
	ProcessMessage.cpp \
	ProcessPOSIXLog.cpp

TGHDRS=	AttrList \
	Attrs \
	DiagnosticCommonKinds \
	DeclNodes \
	StmtNodes \
	CommentCommandList

.include "../lldb.lib.mk"
