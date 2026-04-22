# $FreeBSD: releng/11.0/sys/boot/uboot/Makefile 273934 2014-11-01 17:12:44Z andrew $

.include <src.opts.mk>

SUBDIR=	lib

.if ${MK_FDT} != "no"
SUBDIR+=fdt
.endif

.include <bsd.subdir.mk>
