# $FreeBSD: releng/11.0/usr.bin/clang/llvm-dis/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

PROG_CXX=llvm-dis

SRCDIR=	tools/llvm-dis
SRCS=	llvm-dis.cpp

TGHDRS=	Attributes \
	Intrinsics

LIBDEPS=llvmanalysis \
	llvmbitreader \
	llvmcore \
	llvmsupport

.include "../clang.prog.mk"
