# $FreeBSD: releng/11.0/lib/libclang_rt/asan_cxx/Makefile 288943 2015-10-06 17:53:29Z dim $

.include <bsd.init.mk>

LIB=	clang_rt.asan_cxx-${CRTARCH}

CXXFLAGS+=-frtti

.PATH:	${CRTSRC}/lib/asan\
	${CRTSRC}/lib/ubsan

SRCS=	asan_new_delete.cc\
	ubsan_handlers_cxx.cc\
	ubsan_type_hash.cc\
	ubsan_type_hash_itanium.cc\
	ubsan_type_hash_win.cc

.include <bsd.lib.mk>
