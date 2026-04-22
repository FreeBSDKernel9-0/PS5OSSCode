# $FreeBSD: releng/11.0/sys/modules/rdma/krping/Makefile 290135 2015-10-29 08:28:39Z hselasky $
.PATH: ${.CURDIR}/../../../contrib/rdma/krping

KMOD= krping
SRCS= krping.c krping_dev.c getopt.c
SRCS+=  bus_if.h device_if.h pci_if.h pcib_if.h vnode_if.h
SRCS+=  opt_sched.h opt_inet.h opt_inet6.h
CFLAGS+= -I${.CURDIR}/../../../ofed/include
CFLAGS+= -I${.CURDIR}/../../../compat/linuxkpi/common/include

.include <bsd.kmod.mk>
