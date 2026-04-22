# $FreeBSD: releng/11.0/sys/modules/iwmfw/Makefile.inc 286442 2015-08-08 06:08:20Z rpaulo $
#
# Common rules for building firmware.  Note this gets auto-included
# by the subdir Makefile's as a consequence of included bsd.kmod.mk.

_FIRM=	${IMG}.fw

CLEANFILES+=	${_FIRM}

FIRMWS=	${_FIRM}:${KMOD}

#
# Note that a license ack is not needed for iwn.
#
#FIRMWARE_LICENSE=

${_FIRM}: ${.CURDIR}/../../../contrib/dev/iwm/${_FIRM}.uu
	uudecode -p $? > ${.TARGET}
