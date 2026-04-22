# $FreeBSD: releng/11.0/lib/libclang_rt/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <src.opts.mk>

.if ${MACHINE_CPUARCH} == "i386" || ${MACHINE_CPUARCH} == "amd64"
SUBDIR+= include\
	 asan\
	 asan-preinit\
	 asan_dynamic\
	 asan_cxx\
	 safestack\
	 ubsan_standalone\
	 ubsan_standalone_cxx
.endif

.if ${MACHINE_CPUARCH} == "i386" || ${MACHINE_CPUARCH} == "amd64" || \
    (${MACHINE_CPUARCH} == "arm" && ${MACHINE_ARCH} != "armeb")
SUBDIR+= profile
.endif

.include <bsd.subdir.mk>
