# $FreeBSD: releng/11.0/sys/modules/geom/geom_bde/Makefile 292782 2015-12-27 17:33:59Z allanjude $

.PATH: ${.CURDIR}/../../../geom/bde
.PATH: ${.CURDIR}/../../../crypto/rijndael
.PATH: ${.CURDIR}/../../../crypto/sha2

KMOD=	geom_bde
SRCS=	g_bde.c g_bde_crypt.c g_bde_lock.c g_bde_work.c
SRCS+=	rijndael-alg-fst.c rijndael-api-fst.c sha256c.c sha512c.c

.include <bsd.kmod.mk>
