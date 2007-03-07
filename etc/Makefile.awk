#
#       srecord - manipulate eprom load files
#       Copyright (C) 1998, 2006, 2007 Peter Miller
#
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 2 of the License, or
#       (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with this program; if not, write to the Free Software
#       Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
#
# MANIFEST: helper file for generating Makefile file
#
length <= 72
length > 72 {
        if (substr($0, 1, 1) == "\t")
        {
                printf "\t"
                pos = 8
        }
        else
                pos = 0
        for (j = 1; j <= NF; ++j)
        {
                if (pos + 1 + length($j) > 72)
                {
                        printf "\\\n\t\t"
                        pos = 16
                }
                printf "%s ", $j
                pos += length($j) + 1
        }
        printf "\n"
}
