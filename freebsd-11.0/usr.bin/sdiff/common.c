/*	$OpenBSD: common.c,v 1.4 2006/05/25 03:20:32 ray Exp $	*/

/*
 * Written by Raymond Lai <ray@cyth.net>.
 * Public domain.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: releng/11.0/usr.bin/sdiff/common.c 298823 2016-04-29 23:27:15Z bapt $");

#include <err.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"

void
cleanup(const char *filename)
{

	if (unlink(filename))
		err(2, "could not delete: %s", filename);
	exit(2);
}
