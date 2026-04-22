# $FreeBSD: releng/11.0/usr.bin/bmake/Makefile.inc 288154 2015-09-23 21:35:58Z bdrewery $ 

.sinclude <src.opts.mk>

.if defined(.PARSEDIR)
# make sure this is available to unit-tests/Makefile
.export SRCTOP
.endif

.if exists(${.CURDIR}/tests)
PROG= make
.endif

.if !defined(MK_SHARED_TOOLCHAIN) || ${MK_SHARED_TOOLCHAIN} == "no"
NO_SHARED?=     YES
.endif

# hack to not add tests to tests subdir since this is included from
# there and to avoid renaming things that require changes to generated
# files.
.if defined(MK_TESTS) && ${MK_TESTS} != no && exists(${.CURDIR}/tests)
SUBDIR+= tests
.endif

WARNS=3
CFLAGS+= -DNO_PWD_OVERRIDE
