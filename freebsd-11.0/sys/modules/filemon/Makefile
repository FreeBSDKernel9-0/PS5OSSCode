# $FreeBSD: releng/11.0/sys/modules/filemon/Makefile 295017 2016-01-28 21:45:25Z bdrewery $

.PATH: ${.CURDIR}/../../dev/filemon

KMOD=	filemon
SRCS=	${KMOD}.c
SRCS+=	opt_compat.h vnode_if.h

.include <bsd.kmod.mk>
