# $FreeBSD: releng/11.0/lib/libclang_rt/include/Makefile 291455 2015-11-29 16:28:40Z dim $

.include <bsd.init.mk>

.PATH: ${CRTSRC}/include/sanitizer

INCSDIR=${CLANGDIR}/include/sanitizer

INCS=	allocator_interface.h\
	asan_interface.h\
	common_interface_defs.h\
	coverage_interface.h\
	dfsan_interface.h\
	linux_syscall_hooks.h\
	lsan_interface.h\
	msan_interface.h\
	tsan_interface_atomic.h

.include <bsd.prog.mk>
