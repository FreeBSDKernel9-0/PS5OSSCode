#	@(#)Makefile	8.1 (Berkeley) 6/4/93
# $FreeBSD: releng/11.0/libexec/tftpd/Makefile 275077 2014-11-25 21:18:18Z bapt $

PROG=	tftpd
MAN=	tftpd.8
SRCS=	tftp-file.c tftp-io.c tftp-options.c tftp-transfer.c tftp-utils.c
SRCS+=	tftpd.c
WFORMAT=0

LIBADD=	wrap

.include <bsd.prog.mk>
