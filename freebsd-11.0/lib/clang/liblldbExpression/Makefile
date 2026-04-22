# $FreeBSD: releng/11.0/lib/clang/liblldbExpression/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

LIB=	lldbExpression

SRCDIR=	tools/lldb/source/Expression
SRCS=	DWARFExpression.cpp \
	Expression.cpp \
	ExpressionSourceCode.cpp \
	ExpressionVariable.cpp \
	FunctionCaller.cpp \
	IRDynamicChecks.cpp \
	IRExecutionUnit.cpp \
	IRInterpreter.cpp \
	IRMemoryMap.cpp \
	LLVMUserExpression.cpp \
	Materializer.cpp \
	REPL.cpp \
	UserExpression.cpp \
	UtilityFunction.cpp

TGHDRS=	Attributes \
	DiagnosticCommonKinds \
	DeclNodes \
	StmtNodes \
	CommentCommandList \
	AttrList \
	Attrs \
	AttrParsedAttrList \
	DiagnosticFrontendKinds \
	DiagnosticSemaKinds \
	Intrinsics

.include "../lldb.lib.mk"
