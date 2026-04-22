#
# From: Id: Makefile,v 1.2 2004/01/15 16:20:56 pkern Exp
#
# $FreeBSD: releng/11.0/usr.sbin/ipfwpcap/Makefile 275054 2014-11-25 16:57:27Z bapt $
#

PROG=	ipfwpcap

LIBADD=	pcap

MAN=	ipfwpcap.8

.include <bsd.prog.mk>

test:	$(CMD)
	-rm /var/run/ipfwpcap.2000.pid
	./ipfwpcap -d 2000 - | tcpdump -r - -n -s 2000 -X

