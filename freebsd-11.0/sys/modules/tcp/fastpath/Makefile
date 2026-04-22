#
# $FreeBSD: releng/11.0/sys/modules/tcp/fastpath/Makefile 301717 2016-06-09 05:48:34Z ae $
#

.PATH: ${.CURDIR}/../../../netinet/tcp_stacks

KMOD=	fastpath
SRCS=	fastpath.c

SRCS+=	opt_inet.h opt_inet6.h opt_ipsec.h
SRCS+=	opt_tcpdebug.h

#
# Enable full debugging
#
#CFLAGS += -g

.include <bsd.kmod.mk>
