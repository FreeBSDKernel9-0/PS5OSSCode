# $FreeBSD: releng/11.0/usr.sbin/wpa/hostapd_cli/Makefile 275054 2014-11-25 16:57:27Z bapt $

.include "${.CURDIR}/../Makefile.inc"

.PATH.c:${HOSTAPD_DISTDIR}

PROG=	hostapd_cli
SRCS=	common.c edit.c eloop.c hostapd_cli.c os_unix.c wpa_ctrl.c wpa_debug.c

CFLAGS+= -DCONFIG_CTRL_IFACE
CFLAGS+= -DCONFIG_CTRL_IFACE_UNIX

LIBADD+=	util

MAN=	hostapd_cli.8

.include <bsd.prog.mk>
