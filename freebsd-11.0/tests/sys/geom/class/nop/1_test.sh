#!/bin/sh
# $FreeBSD: releng/11.0/tests/sys/geom/class/nop/1_test.sh 293821 2016-01-13 09:14:27Z ngie $

. `dirname $0`/conf.sh

echo "1..1"

us=$(attach_md -t malloc -s 1M) || exit 1

gnop create /dev/${us} || exit 1

# Size of created device should be 1MB.

size=`diskinfo /dev/${us}.nop | awk '{print $3}'`

if [ $size -eq 1048576 ]; then
	echo "ok 1"
else
	echo "not ok 1"
fi
