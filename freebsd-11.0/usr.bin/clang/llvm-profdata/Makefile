# $FreeBSD: releng/11.0/usr.bin/clang/llvm-profdata/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

PROG_CXX=llvm-profdata

SRCDIR=	tools/llvm-profdata
SRCS=	llvm-profdata.cpp

TGHDRS=	Attributes

LIBDEPS=llvmprofiledata \
	llvmobject \
	llvmmcparser \
	llvmmc \
	llvmbitreader \
	llvmcore \
	llvmsupport

LIBADD+=	z

.include "../clang.prog.mk"
