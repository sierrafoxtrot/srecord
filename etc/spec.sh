#!/bin/sh
#
#	srecord - manipulate eprom load files
#	Copyright (C) 1998, 2001 Peter Miller;
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
echo 'Summary: manipulate EPROM load files'
echo 'Name: srecord'
echo "Version: ${version}"
echo 'Release: 1'
echo 'Copyright: GPL'
echo 'Group: Development/Tools'
echo "Source: http://www.canb.auug.org.au/~millerp/srecord/srecord-${version}.tar.gz"
echo 'URL: http://www.canb.auug.org.au/~millerp/srecord/'
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

The SRecord package is a collection of powerful tools for manipulating
EPROM load files.

* The SRecord package understands a number of file formats: Motorola
  S-Record, Intel, Tektronix, Binary.  These file formats may be read
  and written.  Also C array definitions, for output only.

* The SRecord package has a number of tools: srec_cat for copying and
  and converting files, srec_cmp for comparing files and srec_info for
  printing summaries.

* The SRecord package has a number for filters: checksum to add checksums
  to the data, crop to keep address ranges, exclude to remove address
  ranges, fill to plug holes in the data, length to insert the data
  length, maximum to insert the data address maximum, minimum to insert
  the data address minimum, offset to adjust addresses, and split for
  wide data buses and memory striping.

More than one filter may be applied to each input file.  Different filters
may be applied to each input file.  All filters may be applied to all
file formats.

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
