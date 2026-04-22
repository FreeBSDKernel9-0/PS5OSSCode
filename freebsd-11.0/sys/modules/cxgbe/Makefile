#
# $FreeBSD: releng/11.0/sys/modules/cxgbe/Makefile 292740 2015-12-26 06:05:21Z np $
#

SYSDIR?=${.CURDIR}/../..
.include "${SYSDIR}/conf/kern.opts.mk"

SUBDIR=	if_cxgbe
SUBDIR+=	if_cxl
SUBDIR+=	t4_firmware
SUBDIR+=	t5_firmware
SUBDIR+=	${_tom}
SUBDIR+=	${_iw_cxgbe}
SUBDIR+=	${_cxgbei}

.if ${MACHINE_CPUARCH} == "amd64"
_tom=	tom
_cxgbei=	cxgbei
.if ${MK_OFED} != "no" || defined(ALL_MODULES)
_iw_cxgbe=	iw_cxgbe
.endif
.endif

.if ${MACHINE_CPUARCH} == "i386"
_tom=	tom
.endif

.include <bsd.subdir.mk>
