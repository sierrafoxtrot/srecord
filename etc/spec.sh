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
# MANIFEST: shell script to generate RedHat spec file
#
version=${version-0.0.0}
echo 'Summary: manipulate eprom load files'
echo 'Name: srecord'
echo "Version: ${version}"
echo 'Release: 1'
echo 'Copyright: GPL'
echo 'Group: Development/Building'
echo "Source: http://www.canb.auug.org.au/~millerp/srecord-${version}.tar.gz"
echo 'URL: http://www.canb.auug.org.au/~millerp/srecord.html'
echo 'BuildRoot: /tmp/srecord-build-root'

prefix=/usr
#
# RPM only has install-time relocatable packages.  It has no support for
# build-time relocatable packages.  Therefore, we must NOT specify a Spec
# prefix, or the installed locations will not match the built locations.
#
#echo "Prefix: $prefix"

echo ''

cat << 'fubar'
%description
SRecord is a tool for constructing files. It is given a set of files to
create, and recipes of how to create them. In any non-trivial program
there will be prerequisites to performing the actions necessary to
creating any file, such as include files.  The srecord program provides a
mechanism to define these.

When a program is being developed or maintained, the programmer will
typically change one file of several which comprise the program.  SRecord
examines the last-modified times of the files to see when the
prerequisites of a file have changed, implying that the file needs to be
recreated as it is logically out of date.

SRecord also provides a facility for implicit recipes, allowing users to
specify how to form a file with a given suffix from a file with a
different suffix.  For example, to create filename.o from filename.c

* SRecord has a simple but powerful string-based description language with
  many built-in functions.  This allows sophisticated filename
  specification and manipulation without loss of readability or
  performance.

* SRecord is able to use fingerprints to supplement file modification
  times.  This allows build optimization without contorted rules.

* SRecord is able to build your project with multiple parallel threads,
  with support for rules which must be single threaded.  It is possible
  to distribute parallel builds over your LAN, allowing you to turn your
  network into a virtual parallel build engine.

%prep
fubar

#
# set the prefix here
#
echo '%setup'
echo "./configure --prefix=$prefix"
echo ''
echo '%build'
echo 'make'
echo ''
echo '%install'
echo 'make RPM_BUILD_ROOT=$RPM_BUILD_ROOT install'

#
# remember things for the %files section
#
files=

remember_prog()
{
	if eval "test \"\${prog_${1}-no}\" != yes"
	then
		eval "prog_${1}=yes"
		files="$files $prefix/bin/${1}"
	fi
}

for file in $*
do
	case $file in

	prog/*/main.cc)
		dir=`echo $file | sed 's|prog/\([^/]*\)/.*|\1|'`
		remember_prog $dir
		;;

	man/man[0-9]/*.[0-9])
		files="$files $prefix/$file"
		;;

	*)
		;;
	esac
done

echo ''
echo '%files'
for file in $files
do
	echo "$file"
done | sort
