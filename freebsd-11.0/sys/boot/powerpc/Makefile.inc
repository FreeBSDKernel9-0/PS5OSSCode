# $FreeBSD: releng/11.0/sys/boot/powerpc/Makefile.inc 291667 2015-12-03 00:08:50Z nwhitehorn $

.if ${MACHINE_ARCH} == "powerpc64"
CFLAGS+=	-m32 -mcpu=powerpc
.endif

.include "../Makefile.inc"
