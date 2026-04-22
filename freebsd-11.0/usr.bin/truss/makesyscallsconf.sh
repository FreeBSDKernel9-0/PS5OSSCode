#! /bin/sh
# $FreeBSD: releng/11.0/usr.bin/truss/makesyscallsconf.sh 289239 2015-10-13 18:23:51Z bdrewery $

ABI="$1"
CONF="$2"

header="${ABI}_syscalls.h"

cat > "${CONF}" << EOF
sysnames="${header}.tmp"
sysproto="/dev/null"
sysproto_h="/dev/null"
syshdr="/dev/null"
sysmk="/dev/null"
syssw="/dev/null"
syshide="/dev/null"
syscallprefix="SYS_"
switchname="sysent"
namesname="syscallnames"
systrace="/dev/null"
EOF
