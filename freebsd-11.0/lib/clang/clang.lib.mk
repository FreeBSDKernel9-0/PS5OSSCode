# $FreeBSD: releng/11.0/lib/clang/clang.lib.mk 296417 2016-03-05 20:24:31Z dim $

LLVM_SRCS= ${.CURDIR}/../../../contrib/llvm

.include "clang.build.mk"

INTERNALLIB=

.if ${MACHINE_CPUARCH} == "arm"
STATIC_CXXFLAGS+= -mlong-calls
.endif

.include <bsd.lib.mk>
