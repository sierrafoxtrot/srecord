#!/bin/sh
#
#       srecord - Manipulate EPROM load files
#       Copyright (C) 2018 Scott Finneran
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
#

# Build the reference manual index. Would be nice to push this into etc/documentation.cmake
workingdir=$1
shift
target=$1
shift
manpages=$@
cd $workingdir
awk -f etc/ref-ptx1.awk $manpages | \
    ptx -O -r -w 1000 -ietc/ref-ptx.ignore | \
    sort  -f -d -t'"' +5 -6 +3 -4 | awk -F'"' -f doc/script/ref-ptx2.awk > $target
