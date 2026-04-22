# $FreeBSD: releng/11.0/cddl/usr.bin/zlook/Makefile 264400 2014-04-13 05:21:56Z imp $

.PATH: ${.CURDIR}/../../contrib/opensolaris/cmd/zlook

PROG=	zlook
MAN=

WARNS?=	0
CFLAGS+= -I${.CURDIR}/../../../sys/cddl/compat/opensolaris
#CFLAGS+= -I${.CURDIR}/../../compat/opensolaris/include
#CFLAGS+= -I${.CURDIR}/../../compat/opensolaris/lib/libumem
#CFLAGS+= -I${.CURDIR}/../../contrib/opensolaris/lib/libzfs/common
#CFLAGS+= -I${.CURDIR}/../../contrib/opensolaris/lib/libzpool/common
#CFLAGS+= -I${.CURDIR}/../../contrib/opensolaris/lib/libnvpair
#CFLAGS+= -I${.CURDIR}/../../../sys/cddl/contrib/opensolaris/uts/common/fs/zfs
#CFLAGS+= -I${.CURDIR}/../../../sys/cddl/contrib/opensolaris/uts/common/sys
CFLAGS+= -I${.CURDIR}/../../../sys/cddl/contrib/opensolaris/uts/common
#CFLAGS+= -I${.CURDIR}/../../contrib/opensolaris/head
#CFLAGS+= -I${.CURDIR}/../../lib/libumem
#
#DPADD=	${LIBAVL} ${LIBGEOM} ${LIBM} ${LIBNVPAIR} ${LIBUMEM} ${LIBUUTIL} \
#	${LIBZFS} ${LIBZPOOL}
#LDADD=	-lavl -lgeom -lm -lnvpair -lumem -luutil -lzfs -lzpool

.include <bsd.prog.mk>
