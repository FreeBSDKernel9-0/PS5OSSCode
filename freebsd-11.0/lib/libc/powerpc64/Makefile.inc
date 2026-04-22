# $FreeBSD: releng/11.0/lib/libc/powerpc64/Makefile.inc 281004 2015-04-02 21:18:11Z emaste $

SRCS+=	trivial-vdso_tc.c

# Long double is 64-bits
MDSRCS+=machdep_ldisd.c
SYM_MAPS+=${LIBC_SRCTOP}/powerpc64/Symbol.map
