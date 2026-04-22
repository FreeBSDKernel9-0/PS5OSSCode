# $FreeBSD: releng/11.0/sys/modules/otusfw/otusfw_main/Makefile 288253 2015-09-26 07:08:35Z adrian $

KMOD=	otusfw_main
FIRMWS=	otusfw_main:otusfw_main:1

CLEANFILES=	otusfw_main

otusfw_main: ${.CURDIR}/../../../contrib/dev/otus/otus-main
	cp ${.CURDIR}/../../../contrib/dev/otus/otus-main ${.TARGET}

.include <bsd.kmod.mk>
