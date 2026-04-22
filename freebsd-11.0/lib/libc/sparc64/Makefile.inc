# $FreeBSD: releng/11.0/lib/libc/sparc64/Makefile.inc 281004 2015-04-02 21:18:11Z emaste $
#
# Machine dependent definitions for the ultra sparc architecture.
#

.include "fpu/Makefile.inc"

SRCS+=	trivial-vdso_tc.c

# Long double is quad precision
GDTOASRCS+=strtorQ.c
MDSRCS+=machdep_ldisQ.c
SYM_MAPS+=${LIBC_SRCTOP}/sparc64/Symbol.map
