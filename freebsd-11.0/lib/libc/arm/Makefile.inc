# $FreeBSD: releng/11.0/lib/libc/arm/Makefile.inc 300119 2016-05-18 06:01:18Z imp $
#
# Machine dependent definitions for the arm architecture.
#

SOFTFLOAT_BITS=32

# Long double is just double precision.
MDSRCS+=machdep_ldisd.c
SYM_MAPS+=${LIBC_SRCTOP}/arm/Symbol.map

.include "${LIBC_SRCTOP}/arm/aeabi/Makefile.inc"

.if ${MACHINE_ARCH:Marmv6*} && (!defined(CPUTYPE) || ${CPUTYPE:M*soft*} == "")
SYM_MAPS+=${LIBC_SRCTOP}/arm/Symbol_vfp.map
.endif

