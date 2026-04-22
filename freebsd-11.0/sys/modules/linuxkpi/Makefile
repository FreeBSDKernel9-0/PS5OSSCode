# $FreeBSD: releng/11.0/sys/modules/linuxkpi/Makefile 290135 2015-10-29 08:28:39Z hselasky $
.PATH:	${.CURDIR}/../../compat/linuxkpi/common/src

KMOD=	linuxkpi
SRCS=	linux_kmod.c \
	linux_compat.c \
	linux_pci.c \
	linux_radix.c \
	linux_idr.c \
	linux_usb.c

SRCS+=	bus_if.h \
	device_if.h \
	pci_if.h \
	vnode_if.h \
	usb_if.h \
	opt_usb.h

CFLAGS+= -I${.CURDIR}/../../compat/linuxkpi/common/include

.include <bsd.kmod.mk>
