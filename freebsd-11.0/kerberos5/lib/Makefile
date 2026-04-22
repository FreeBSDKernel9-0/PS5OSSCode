
# $FreeBSD: releng/11.0/kerberos5/lib/Makefile 289251 2015-10-13 18:52:56Z bdrewery $

SUBDIR=	libasn1 libgssapi_krb5 libgssapi_ntlm libgssapi_spnego libhdb \
	libheimntlm libhx509 libkadm5clnt libkadm5srv libkrb5 \
	libroken libsl libvers libkdc libwind libheimbase libheimipcc libheimipcs

SUBDIR+= libkafs5  # requires krb_err.h from libkrb5
SUBDIR_DEPEND_libkafs5=	libkrb5

.include <bsd.subdir.mk>
