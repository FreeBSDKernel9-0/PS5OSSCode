# $FreeBSD: releng/11.0/sys/modules/drm/Makefile 276639 2015-01-03 22:36:18Z jhibbits $

SYSDIR?=${.CURDIR}/../..
.include "${SYSDIR}/conf/kern.opts.mk"

# Modules that include binary-only blobs of microcode should be selectable by
# MK_SOURCELESS_UCODE option (see below).

SUBDIR = \
	drm \
	i915 \
	mach64 \
	${_mga} \
	${_r128} \
	${_radeon} \
	savage \
	sis \
	tdfx \
	via

.if ${MK_SOURCELESS_UCODE} != "no"
_mga=		mga
_r128=		r128
.if ${MACHINE_CPUARCH} != "powerpc"
_radeon=	radeon
.endif
.endif

.include <bsd.subdir.mk>
