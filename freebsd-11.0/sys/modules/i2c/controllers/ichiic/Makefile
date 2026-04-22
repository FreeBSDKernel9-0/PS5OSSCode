#$FreeBSD: releng/11.0/sys/modules/i2c/controllers/ichiic/Makefile 283766 2015-05-30 12:17:18Z grembo $

.PATH:		${.CURDIR}/../../../../dev/ichiic
KMOD		= ig4
SRCS		= device_if.h bus_if.h iicbb_if.h pci_if.h smbus_if.h \
		  ig4_iic.c ig4_pci.c ig4_reg.h ig4_var.h

.include <bsd.kmod.mk>
