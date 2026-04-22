# $FreeBSD: releng/11.0/lib/libclang_rt/profile/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.init.mk>

LIB=	clang_rt.profile-${CRTARCH}

.PATH:	${CRTSRC}/lib/profile

SRCS=	GCDAProfiling.c\
	InstrProfiling.c\
	InstrProfilingValue.c\
	InstrProfilingBuffer.c\
	InstrProfilingFile.c\
	InstrProfilingWriter.c\
	InstrProfilingPlatformDarwin.c\
	InstrProfilingPlatformLinux.c\
	InstrProfilingPlatformOther.c\
	InstrProfilingRuntime.cc\
	InstrProfilingUtil.c

.include <bsd.lib.mk>
