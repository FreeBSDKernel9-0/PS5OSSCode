# $FreeBSD: releng/11.0/lib/libc/riscv/Makefile.inc 294227 2016-01-17 15:21:23Z br $
#
# Machine dependent definitions for the RISC-V architecture.
#

# Long double is quad precision
GDTOASRCS+=strtorQ.c
MDSRCS+=machdep_ldisQ.c
SYM_MAPS+=${LIBC_SRCTOP}/riscv/Symbol.map
