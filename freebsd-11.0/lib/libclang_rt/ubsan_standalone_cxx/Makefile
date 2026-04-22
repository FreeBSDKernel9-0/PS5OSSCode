# $FreeBSD: releng/11.0/lib/libclang_rt/ubsan_standalone_cxx/Makefile 287626 2015-09-10 20:35:47Z dim $

.include <bsd.init.mk>

LIB=	clang_rt.ubsan_standalone_cxx-${CRTARCH}

CXXFLAGS+=-frtti

.PATH:	${CRTSRC}/lib/ubsan

SRCS=	ubsan_handlers_cxx.cc\
	ubsan_type_hash.cc\
	ubsan_type_hash_itanium.cc\
	ubsan_type_hash_win.cc

.include <bsd.lib.mk>
