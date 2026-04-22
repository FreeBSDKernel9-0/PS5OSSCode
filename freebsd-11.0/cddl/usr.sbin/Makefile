# $FreeBSD: releng/11.0/cddl/usr.sbin/Makefile 300906 2016-05-28 17:43:40Z asomers $

.include <src.opts.mk>

SUBDIR=	${_dtrace} \
	${_lockstat} \
	${_plockstat} \
	${_tests} \
	${_zdb} \
	${_zfsd} \
	${_zhack}

.if ${MK_TESTS} != "no"
_tests=	tests
.endif

.if ${MK_ZFS} != "no"
.if ${MK_LIBTHR} != "no"
_zdb=	zdb
_zhack=	zhack
.endif
. if ${MK_CXX} != "no"
_zfsd=	zfsd
. endif
.endif

.if ${MACHINE_ARCH} == "amd64" || ${MACHINE_ARCH} == "i386"
_dtrace=	dtrace
_lockstat=	lockstat
_plockstat=	plockstat
.endif

.if ${MACHINE_CPUARCH} == "aarch64" || ${MACHINE_CPUARCH} == "arm" || \
    ${MACHINE_CPUARCH} == "riscv"
_dtrace=	dtrace
_lockstat=	lockstat
.endif

.if ${MACHINE_CPUARCH} == "mips"
_dtrace=	dtrace
.endif

.if ${MACHINE_CPUARCH} == "powerpc"
_dtrace=	dtrace
_lockstat=	lockstat
.endif

SUBDIR_PARALLEL=

.include <bsd.subdir.mk>
