# $FreeBSD: releng/11.0/usr.bin/clang/llvm-cov/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

PROG_CXX=llvm-cov

SRCDIR=	tools/llvm-cov
SRCS=	CodeCoverage.cpp \
	CoverageFilters.cpp \
	CoverageReport.cpp \
	CoverageSummaryInfo.cpp \
	SourceCoverageView.cpp \
	TestingSupport.cpp \
	gcov.cpp \
	llvm-cov.cpp

LIBDEPS=llvmprofiledata \
	llvmobject \
	llvmmcparser \
	llvmmc \
	llvmbitreader \
	llvmcore \
	llvmsupport

LIBADD+=	z

.include "../clang.prog.mk"
