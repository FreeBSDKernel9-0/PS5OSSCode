# $FreeBSD: releng/11.0/usr.bin/clang/llvm-dwarfdump/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

PROG_CXX=llvm-dwarfdump

SRCDIR=	tools/llvm-dwarfdump
SRCS=	llvm-dwarfdump.cpp

LIBDEPS=llvmdebuginfodwarf \
	llvmobject \
	llvmmcparser \
	llvmmc \
	llvmbitreader \
	llvmcore \
	llvmsupport

LIBADD+=	z

.include "../clang.prog.mk"
