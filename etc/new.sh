#
# srecord - manipulate eprom load files
# Copyright (C) 1998, 2006, 2007 Peter Miller
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
        sortflag=r
        shift
fi
echo $* |
tr ' ' '\12' |
sort -t. +1n$sortflag -2 +2n$sortflag -3 +3n$sortflag -5 |
while read f
do
        echo ".br"
        echo ".ne 3i"
        echo ".so $f"
done
exit 0
# vim:ts=8:sw=4:et
