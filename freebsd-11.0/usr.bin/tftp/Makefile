#	@(#)Makefile	8.1 (Berkeley) 6/6/93
# $FreeBSD: releng/11.0/usr.bin/tftp/Makefile 275042 2014-11-25 14:29:10Z bapt $

.PATH: ${.CURDIR}/../../libexec/tftpd

PROG=	tftp
SRCS=	main.c tftp-file.c tftp-io.c tftp-options.c tftp-transfer.c
SRCS+=	tftp-utils.c tftp.c
CFLAGS+=-I${.CURDIR}/../../libexec/tftpd

LIBADD=	edit

.include <bsd.prog.mk>
