# $FreeBSD: releng/11.0/usr.bin/clang/llvm-as/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

PROG_CXX=llvm-as

SRCDIR=	tools/llvm-as
SRCS=	llvm-as.cpp

TGHDRS=	Attributes

LIBDEPS=llvmbitwriter \
	llvmasmparser \
	llvmcore \
	llvmsupport

.include "../clang.prog.mk"
