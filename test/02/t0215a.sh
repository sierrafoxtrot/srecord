#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2011 Peter Miller
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

TEST_SUBJECT="write stag ppb"
. test_prelude.sh

cat > test.in << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
00000000: 01 00 00 00 0E 00 00 00 00 48 65 6C 6C 6F 2C 20  #.........Hello,
00000010: 57 6F 72 6C 64 21 0A 7F 01 00 00 00 00 00 00 00  #World!..........
00000020: 00 00                                            #..
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out.2 -ppb
if test $? -ne 0; then fail; fi

srec_cat test.out.2 -bin -o test.out -hexdump
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
