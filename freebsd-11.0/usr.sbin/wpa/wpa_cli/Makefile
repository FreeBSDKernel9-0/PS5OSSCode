# $FreeBSD: releng/11.0/usr.sbin/wpa/wpa_cli/Makefile 275054 2014-11-25 16:57:27Z bapt $

.include "${.CURDIR}/../Makefile.inc"

.PATH.c:${WPA_SUPPLICANT_DISTDIR}

PROG=	wpa_cli
SRCS=	common.c edit.c eloop.c os_unix.c wpa_cli.c wpa_ctrl.c wpa_debug.c

MAN=	wpa_cli.8

CFLAGS+= -DCONFIG_CTRL_IFACE
CFLAGS+= -DCONFIG_CTRL_IFACE_UNIX
# enable use of d_type to identify unix domain sockets
CFLAGS+= -D_DIRENT_HAVE_D_TYPE

CFLAGS+= -DCONFIG_WPA_CLI_EDIT=y
LIBADD+=	util

.include <bsd.prog.mk>
