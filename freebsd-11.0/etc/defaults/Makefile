# $FreeBSD: releng/11.0/etc/defaults/Makefile 296695 2016-03-11 23:45:09Z bdrewery $

.include <src.opts.mk>

FILES=	devfs.rules periodic.conf rc.conf
FILESDIR= /etc/defaults

.if ${MK_BLUETOOTH} != "no"
FILES+=	bluetooth.device.conf
.endif

.include <bsd.prog.mk>
