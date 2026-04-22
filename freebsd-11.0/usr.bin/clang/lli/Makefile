# $FreeBSD: releng/11.0/usr.bin/clang/lli/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

PROG_CXX=lli

SRCDIR=	tools/lli
SRCS=	OrcLazyJIT.cpp \
	lli.cpp

TGHDRS=	Attributes \
	Intrinsics

LIBDEPS=llvmx86disassembler \
	llvmx86asmparser \
	llvmirreader \
	llvmasmparser \
	llvmx86codegen \
	llvmx86desc \
	llvmx86info \
	llvmmcdisassembler \
	llvmx86asmprinter \
	llvmx86utils \
	llvmselectiondag \
	llvmasmprinter \
	llvminterpreter \
	llvmcodegen \
	llvmscalaropts \
	llvminstcombine \
	llvmbitwriter \
	llvminstrumentation \
	llvmprofiledata \
	llvmorcjit \
	llvmtransformutils \
	llvmmcjit \
	llvmexecutionengine \
	llvmtarget \
	llvmanalysis \
	llvmruntimedyld \
	llvmobject \
	llvmmcparser \
	llvmbitreader \
	llvmmc \
	llvmcore \
	llvmsupport

LIBADD+=	z

.include "../clang.prog.mk"
