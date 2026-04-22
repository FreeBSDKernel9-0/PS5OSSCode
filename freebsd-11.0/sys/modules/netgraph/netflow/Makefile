# $FreeBSD: releng/11.0/sys/modules/netgraph/netflow/Makefile 269540 2014-08-04 22:37:02Z imp $
#
# Author: Gleb Smirnoff <glebius@freebsd.org>
#

.PATH: ${.CURDIR}/../../../netgraph/netflow

KMOD=   ng_netflow
SRCS=   ng_netflow.c netflow.c netflow_v9.c opt_inet6.h opt_route.h

.include <bsd.kmod.mk>
