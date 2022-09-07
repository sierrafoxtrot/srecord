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
as=$1
in=$2
out=$3

pages=`awk -f $as $in`
es=$?
test $es = 0 || exit $es

psselect -p$pages $in > $out
es=$?
test $es = 0 || exit $es
