#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2012, 2014 Peter Miller
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

TEST_SUBJECT="Lattice .mem output"
. test_prelude.sh

cat > test.in << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
#Format=Hex
#Depth=4
#Width=32
#AddrRadix=3
#DataRadix=3
#Data
#
# Generated automatically by srec_cat -o --MEM 32
#
# HDR
48656C6C
6F2C2057
6F726C64
210A0000
# execution start address = 0000
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in --fill 0 0 16 -o test.out -mem 32
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
