# $FreeBSD: releng/11.0/usr.bin/clang/Makefile 301993 2016-06-17 16:46:58Z bdrewery $

.include <src.opts.mk>

SUBDIR=	clang clang-tblgen llvm-tblgen

.if !defined(TOOLS_PREFIX)
.if ${MK_CLANG_EXTRAS} != "no"
SUBDIR+=bugpoint \
	clang-format \
	llc \
	lli \
	llvm-ar \
	llvm-as \
	llvm-bcanalyzer \
	llvm-cov \
	llvm-cxxdump \
	llvm-diff \
	llvm-dis \
	llvm-dwarfdump \
	llvm-extract \
	llvm-link \
	llvm-lto \
	llvm-mc \
	llvm-nm \
	llvm-objdump \
	llvm-pdbdump \
	llvm-profdata \
	llvm-rtdyld \
	llvm-symbolizer \
	opt
.endif

.if ${MK_LLDB} != "no"
SUBDIR+=lldb
.endif
.endif # TOOLS_PREFIX

SUBDIR_PARALLEL=

.include <bsd.subdir.mk>
