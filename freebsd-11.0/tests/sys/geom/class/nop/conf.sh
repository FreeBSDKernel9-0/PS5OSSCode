#!/bin/sh
# $FreeBSD: releng/11.0/tests/sys/geom/class/nop/conf.sh 293437 2016-01-08 19:43:18Z ngie $

class="nop"
base=`basename $0`

gnop_test_cleanup()
{
	[ -c /dev/${us}.nop ] && gnop destroy ${us}.nop
	geom_test_cleanup
}
trap gnop_test_cleanup ABRT EXIT INT TERM

. `dirname $0`/../geom_subr.sh
