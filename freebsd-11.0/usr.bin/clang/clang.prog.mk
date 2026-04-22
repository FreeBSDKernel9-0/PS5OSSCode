# $FreeBSD: releng/11.0/usr.bin/clang/clang.prog.mk 298107 2016-04-16 07:45:30Z gjb $

LLVM_SRCS= ${.CURDIR}/../../../contrib/llvm

.include "../../lib/clang/clang.build.mk"

.for lib in ${LIBDEPS}
DPADD+=	${.OBJDIR}/../../../lib/clang/lib${lib}/lib${lib}.a
LDADD+=	${.OBJDIR}/../../../lib/clang/lib${lib}/lib${lib}.a
.endfor

PACKAGE=	clang

LIBADD+= ncursesw pthread

BINDIR?= /usr/bin


.if ${MK_SHARED_TOOLCHAIN} == "no"
NO_SHARED= yes
.endif

.include <bsd.prog.mk>
