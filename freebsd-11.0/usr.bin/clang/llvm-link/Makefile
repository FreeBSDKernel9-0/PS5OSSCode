# $FreeBSD: releng/11.0/usr.bin/clang/llvm-link/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

PROG_CXX=llvm-link

SRCDIR=	tools/llvm-link
SRCS=	llvm-link.cpp

TGHDRS=	Attributes

LIBDEPS=llvmobject \
	llvmmcparser \
	llvmmc \
	llvmirreader \
	llvmasmparser \
	llvmbitwriter \
	llvmbitreader \
	llvmlinker \
	llvmtransformutils \
	llvmanalysis \
	llvmcore \
	llvmsupport

.include "../clang.prog.mk"
