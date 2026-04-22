# $FreeBSD: releng/11.0/sbin/fdisk_pc98/Makefile 298107 2016-04-16 07:45:30Z gjb $

PACKAGE=runtime
PROG=	fdisk
SRCS=	fdisk.c geom_pc98_enc.c
WARNS?=	4
MAN=	fdisk.8

.PATH:	${.CURDIR}/../../sys/geom

LIBADD=	geom

.include <bsd.prog.mk>
