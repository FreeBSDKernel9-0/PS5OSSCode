# $FreeBSD: releng/11.0/usr.bin/clang/clang-format/Makefile 301993 2016-06-17 16:46:58Z bdrewery $

MAN=
.include <bsd.own.mk>

PROG_CXX=clang-format

SRCDIR=	tools/clang/tools/clang-format
SRCS=	ClangFormat.cpp

LIBDEPS=clangrewrite \
	clanglex \
	clangformat \
	clangbasic \
	clangast \
	clangtoolingcore \
	llvmmc \
	llvmcore \
	llvmsupport \

TGHDRS=	DiagnosticCommonKinds

.include "../clang.prog.mk"
