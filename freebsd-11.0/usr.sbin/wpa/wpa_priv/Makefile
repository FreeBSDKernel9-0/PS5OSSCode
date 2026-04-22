# $FreeBSD: releng/11.0/usr.sbin/wpa/wpa_priv/Makefile 275054 2014-11-25 16:57:27Z bapt $

.include "${.CURDIR}/../Makefile.inc"

.PATH.c:${WPA_SUPPLICANT_DISTDIR} \
	${WPA_DISTDIR}/src/drivers

PROG=	wpa_priv
SRCS=	drivers.c os_unix.c eloop.c common.c wpa_debug.c wpabuf.c wpa_priv.c \
	driver_common.c l2_packet_freebsd.c

LIBADD=	pcap

.include "${.CURDIR}/../Makefile.crypto"

.include <bsd.prog.mk>
