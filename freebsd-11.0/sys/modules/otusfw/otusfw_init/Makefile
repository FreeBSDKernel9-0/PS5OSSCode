# $FreeBSD: releng/11.0/sys/modules/otusfw/otusfw_init/Makefile 288253 2015-09-26 07:08:35Z adrian $

KMOD=	otusfw_init
FIRMWS=	otusfw_init:otusfw_init:1

CLEANFILES=	otus_init

otusfw_init: ${.CURDIR}/../../../contrib/dev/otus/otus-init
	cp ${.CURDIR}/../../../contrib/dev/otus/otus-init ${.TARGET}

.include <bsd.kmod.mk>
