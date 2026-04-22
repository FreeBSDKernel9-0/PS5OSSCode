# $FreeBSD: releng/11.0/sys/modules/nfslockd/Makefile 269540 2014-08-04 22:37:02Z imp $

.PATH: ${.CURDIR}/../../nlm ${.CURDIR}/../../rpc
KMOD=	nfslockd
SRCS=	vnode_if.h \
	nlm_advlock.c \
	nlm_prot_clnt.c \
	nlm_prot_impl.c \
	nlm_prot_server.c \
	nlm_prot_svc.c \
	nlm_prot_xdr.c \
	sm_inter_xdr.c
SRCS+=	opt_inet6.h opt_nfs.h

.include <bsd.kmod.mk>
