# $FreeBSD: releng/11.0/sys/modules/ips/Makefile 276606 2015-01-03 03:35:18Z imp $

.PATH:		${.CURDIR}/../../dev/ips
KMOD		= ips
SRCS		= ips.c ips_pci.c ips.h ips_disk.c ips_disk.h ips_commands.c \
			ips_ioctl.h ips_ioctl.c device_if.h bus_if.h pci_if.h 

.include <bsd.kmod.mk>

# XXX Work around clang warning, until maintainer approves fix.
CWARNFLAGS.ips.c=	${NO_WSOMETIMES_UNINITIALIZED}
