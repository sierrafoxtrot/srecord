#!/bin/sh
#
#	srecord - manipulate eprom load files
#	Copyright (C) 1998 Peter Miller;
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
# MANIFEST: shell script to generate Makefile fragment for each source file
#
case $# in
2)
	;;
*)
	echo "usage: $0 filename resolved-filename" 1>&2
	exit 1
	;;
esac
file="$1"
rfn="$2"

case $file in

*/*.y)
	root=`basename $file .y`
	stem=`echo $file | sed 's/\.y$//'`
	dir=`dirname $file`

	case $file in
	*)
		numconf="no"
		;;
	esac

	yy=`echo $stem | sed -e 's|^[^/]*/||' -e 's|[^a-zA-Z0-9]|_|g'`
	echo ""
	echo "${stem}.gen.cc ${stem}.gen.h: $file"
	echo "	@echo Expect $numconf conflicts:"
	echo "	\$(YACC) -d $file"
	echo "	sed -e 's/[yY][yY]/${yy}_/g' y.tab.c > ${stem}.gen.cc"
	echo "	sed -e 's/[yY][yY]/${yy}_/g' y.tab.h > ${stem}.gen.h"
	echo "	rm y.tab.c y.tab.h"

	depfile=$file.d
	if [ -r $depfile ]; then
		dep=`sed -e '1d' -e '$d' -e 's_.arch]/__' $depfile`
	else
		depfile=$rfn.d
		if [ -r $depfile ]; then
			dep=`sed -e '1d' -e '$d' -e 's_.arch]/__' $depfile`
		fi
	fi

	echo ""
	echo "${stem}.gen.o: ${stem}.gen.cc" $dep
	echo "	\$(CXX) \$(CPPFLAGS) \$(CXXFLAGS) -I$dir -Iinclude -c ${stem}.gen.cc"
	echo "	mv ${root}.gen.o ${stem}.gen.o"
	;;

*/*.cc)
	root=`basename $file .cc`
	stem=`echo $file | sed 's/\.cc$//'`
	dir=`dirname $file`

	depfile=$file.d
	if [ -r $depfile ]; then
		dep=`sed -e '1d' -e '$d' -e 's_.arch]/__' $depfile`
	else
		depfile=$rfn.d
		if [ -r $depfile ]; then
			dep=`sed -e '1d' -e '$d' -e 's_.arch]/__' $depfile`
		fi
	fi

	echo ""
	echo "${stem}.o: $file" $dep
	echo "	\$(CXX) \$(CPPFLAGS) \$(CXXFLAGS) -I$dir -Iinclude -c $file"
	echo "	mv ${root}.o ${stem}.o"
	;;

man/man[0-9]/*.[0-9])
	dir=`dirname $file`
	stem=`echo $file | sed 's|^man/\(.*\)|\1|'`

	depfile=$file.d
	dep=
	if [ -r $depfile ]; then
		dep=`sed -e '1d' -e '$d' -e 's_.arch]/__' $depfile`
	else
		depfile=$rfn.d
		if [ -r $depfile ]; then
			dep=`sed -e '1d' -e '$d' -e 's_.arch]/__' $depfile`
		fi
	fi

	echo ""
	echo "\$(mandir)/$stem: $file" $dep $dir/.mandir
	echo "	\$(SOELIM) -I$dir -Ietc $file > tmp"
	echo "	\$(INSTALL_DATA) tmp \$@"
	echo "	@rm -f tmp"
	;;

test/*/*.sh)
	root=`basename $file .sh`
	echo ""
	echo "$root: $file all"
	echo "	\$(SH) $file"
	;;

*)
	;;
esac
exit 0
