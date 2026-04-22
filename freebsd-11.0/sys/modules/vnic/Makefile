# $FreeBSD: releng/11.0/sys/modules/vnic/Makefile 300294 2016-05-20 11:00:06Z wma $

SYSDIR?=${.CURDIR}/../..
.include "${SYSDIR}/conf/kern.opts.mk"

CFLAGS+=	-DFDT

SUBDIR = mrmlbus thunder_mdio thunder_bgx vnicpf vnicvf

.include <bsd.subdir.mk>
