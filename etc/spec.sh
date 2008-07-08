#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 1998, 2001, 2003, 2005-2008 Peter Miller
#
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 3 of the License, or
#       (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with this program. If not, see
#       <http://www.gnu.org/licenses/>.
#
echo '#'
echo '# This file is generated.  Please do not send the maintainer'
echo '# patches to this file.  Please update etc/spec.sh and send a'
echo '# patch to that file, instead.'
echo '#'

version=${version-0.0.0}
echo 'Summary: Manipulate EPROM load files'
echo 'Name: srecord'
echo "Version: ${version}"
echo 'Release: 1'
echo 'License: GPL'
echo 'Group: Development/Tools'
echo "Source: http://srecord.sourceforge.net/%{name}-%{version}.tar.gz"
echo 'URL: http://srecord.sourceforge.net/'
echo 'BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)'
echo
echo 'BuildRequires:  diffutils, sharutils, groff, gcc-c++, boost-devel'

prefix=/usr
#
# RPM only has install-time relocatable packages.  It has no support for
# build-time relocatable packages.  Therefore, we must NOT specify a Spec
# prefix, or the installed locations will not match the built locations.
#
#echo "Prefix: $prefix"

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
%setup -q


%build
%configure
make


%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install


%check || :
make sure


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr (-,root,root,-)
%doc LICENSE BUILDING README
fubar

#
# remember things for the %files section
#
files=

remember_prog()
{
        if eval "test \"\${prog_${1}-no}\" != yes"
        then
                eval "prog_${1}=yes"
                files="$files %{_bindir}/${1}"
        fi
}

for file in $*
do
        case $file in

        test_*)
            ;;

        */main.cc)
                dir=`echo $file | sed 's|\([^/]*\)/.*|\1|'`
                remember_prog $dir
                ;;

        man/man[0-9]/*.[0-9])
                ff=`echo $file | sed 's|^man/||'`
                files="$files %{_mandir}/${ff}*"
                ;;

        *)
                ;;
        esac
done

for file in $files
do
        echo "$file"
done | sort
