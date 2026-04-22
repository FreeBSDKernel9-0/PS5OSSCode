# $FreeBSD: releng/11.0/sys/modules/dpt/Makefile 278959 2015-02-18 15:25:22Z imp $

SYSDIR?=${.CURDIR}/../..
.include "${SYSDIR}/conf/kern.opts.mk"

.PATH:	${SYSDIR}/dev/dpt
KMOD=	dpt
SRCS=	dpt_scsi.c dpt.h \
	dpt_pci.c pci_if.h \
	opt_dpt.h opt_eisa.h \
	opt_cam.h opt_scsi.h \
	device_if.h bus_if.h

SRCS.DEV_EISA=	dpt_eisa.c eisa_if.h

.include <bsd.kmod.mk>
