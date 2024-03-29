#!/bin/sh
#
# srecord - manipulate eprom load files
# Copyright (C) 1998, 2006-2008, 2010, 2011 Peter Miller
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or (at
# your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#

sortflag=
if test "$1" = "-r"; then
    sortflag=-r
    shift
fi
echo "$*" |
    tr ' ' '\12' |
    sort $sortflag -V |
    while read -r f; do
        echo ".so $f"
    done
exit 0
