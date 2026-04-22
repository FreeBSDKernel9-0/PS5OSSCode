# $FreeBSD: releng/11.0/sys/modules/patm/Makefile 269540 2014-08-04 22:37:02Z imp $
#
# Author: Harti Brandt <harti@freebsd.org>
#
.PATH: ${.CURDIR}/../../dev/patm

KMOD=	if_patm
SRCS=	if_patm.c if_patm_attach.c if_patm_ioctl.c if_patm_intr.c \
	if_patm_tx.c if_patm_rx.c if_patm_rtables.c \
	device_if.h bus_if.h pci_if.h opt_inet.h opt_natm.h

CFLAGS+= -DENABLE_BPF
# CFLAGS+= -DPATM_DEBUG=0x0 -DINVARIANT_SUPPORT -DINVARIANTS -DDIAGNOSTIC

.include <bsd.kmod.mk>
