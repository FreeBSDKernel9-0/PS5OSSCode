#!/bin/sh
# $FreeBSD: releng/11.0/usr.sbin/chown/tests/chown-f_test.sh 268793 2014-07-17 06:57:26Z eadler $

base=`basename $0`

echo "1..1"

name="chown -f root:wheel file"
if [ `id -u` -eq 0 ]; then
	echo "ok 1 - $name # skip Test must not be uid 0."
else
    touch file
    output=$(chown -f root:wheel file 2>&1)
    if [ $? -eq 0 -a -z "$output" ]
    then
        echo "ok 1 - $name"
    else
        echo "not ok 1 - $name"
    fi
    rm file
fi
