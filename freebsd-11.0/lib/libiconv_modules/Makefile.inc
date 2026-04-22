# $FreeBSD: releng/11.0/lib/libiconv_modules/Makefile.inc 264928 2014-04-25 19:25:26Z imp $

.PATH: ${.CURDIR}/../../libc/iconv

SHLIB_MAJOR= 4
WARNS?=	6
CFLAGS+= -I${.CURDIR}/../../libc/iconv

CFLAGS+=	-Dbool=_Bool

.if !defined(COMPAT_32BIT)
SHLIBDIR= /usr/lib/i18n
.else
SHLIBDIR= /usr/lib32/i18n
.endif
LIBDIR=	 ${SHLIBDIR}
MK_PROFILE=	no
