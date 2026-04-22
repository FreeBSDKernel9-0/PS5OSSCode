# $FreeBSD: releng/11.0/usr.sbin/extattr/Makefile 299085 2016-05-04 20:20:55Z asomers $

PROG=	rmextattr
MAN=	rmextattr.8

LIBADD=	sbuf

LINKS+=	${BINDIR}/rmextattr ${BINDIR}/getextattr
LINKS+=	${BINDIR}/rmextattr ${BINDIR}/setextattr
LINKS+=	${BINDIR}/rmextattr ${BINDIR}/lsextattr

MLINKS+=	rmextattr.8 setextattr.8
MLINKS+=	rmextattr.8 getextattr.8
MLINKS+=	rmextattr.8 lsextattr.8

.include <src.opts.mk>

.if ${MK_TESTS} != "no"
SUBDIR+=	tests
.endif

.include <bsd.prog.mk>
