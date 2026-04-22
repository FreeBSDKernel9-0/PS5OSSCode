# $FreeBSD: releng/11.0/sys/boot/common/Makefile 284590 2015-06-19 05:42:24Z bapt $

.include <src.opts.mk>

MAN+=	loader.8
.if ${MK_ZFS} != "no"
MAN+=	zfsloader.8
.endif

.include <bsd.prog.mk>
