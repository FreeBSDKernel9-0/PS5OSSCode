# $FreeBSD: releng/11.0/cddl/usr.sbin/zfsd/Makefile 300906 2016-05-28 17:43:40Z asomers $

SRCDIR=${.CURDIR}/../../..
.include "Makefile.common"

PROG_CXX=	zfsd
MAN=		zfsd.8

.include <bsd.prog.mk>

# The unittests require devel/googletest and devel/googlemock from ports.
# Don't automatically build them.
SUBDIR=
