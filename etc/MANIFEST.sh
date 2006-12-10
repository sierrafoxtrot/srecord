#!/bin/sh
#
#	srecord - manipulate eprom load files
#	Copyright (C) 1998, 2006 Peter Miller;
#	All rights reserved.
#
#	This program is free software; you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation; either version 2 of the License, or
#	(at your option) any later version.
#
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with this program; if not, write to the Free Software
#	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
#
# MANIFEST: shell script to generate MANIFEST fragment for each source file
#
case $# in
2)
	;;
*)
	echo "usage: $0 filename resolved-filename" 1>&2
	exit 1
	;;
esac
ff=$1
f=$2
case $ff in
etc/MANIFEST.*)
	info="Script for building this file."
	;;
LICENSE)
	info="GNU General Public License"
	;;
README)
	info="Blurb about srecord"
	;;
BUILDING)
	info="Instructions how to build, test and install srecord"
	;;
etc/CHANGES*)
	info="Change history of srecord"
	;;
Makefile)
	info="Instructions to make(1) how to build and test srecord"
	;;
common/patchlevel.h)
	info="The patch level of this distribution."
	;;
etc/version.so)
	info="The patch level of this distribution."
	;;
etc/new.so)
	info="Indirection to release note for this release."
	;;
configure)
	info="Shell script to construct Makefile and lib/config.h"
	;;
Makefile.in)
	info="Template for Makefile, used by configure"
	;;
lib/config.h.in)
	info="Template for lib/config.h, used by configure"
	;;
etc/template/*)
	info="New file template."
	;;
*)
	info=`awk '
/[ 	]MANIFEST:[ 	]/ {
for (j = 1; j < NF; ++j)
	if ($j == "MANIFEST:")
		break;
for (k = j + 1; k <= NF; ++k)
	printf(" %s", $k);
printf("\n");
exit;
}' $f`
	;;
esac
echo "$ff	$info"
exit 0
