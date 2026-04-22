# $FreeBSD: releng/11.0/usr.sbin/mpsutil/Makefile 291002 2015-11-17 20:42:59Z bapt $

PROG=	mpsutil
SRCS=	mps_cmd.c mps_flash.c mps_show.c mpsutil.c
MAN=	mpsutil.8

WARNS?= 3

#LIBADD=	cam util
LINKS=	${BINDIR}/mpsutil ${BINDIR}/mprutil
MLINKS=	mpsutil.8 mprutil.8

CFLAGS+= -I${.CURDIR}/../../sys -I. -DUSE_MPT_IOCTLS
# Avoid dirdep dependency on libutil
CFLAGS+= -I${SRCTOP}/lib/libutil

# Here be dragons
.ifdef DEBUG
CFLAGS+= -DDEBUG
.endif

.include <bsd.prog.mk>
