#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2013 Peter Miller
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

TEST_SUBJECT="output vhdl_textio"
. test_prelude.sh

cat > test.in1 << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S1230000FB0BD0707D37A17B0940F850E3B8ADC5F9DEBECBA8F201C38081EB41A3B9EACE29
S12300205631573C8F926267B25C15F54AF182B222E3CFA4BBDAE85512C391E0B89FC8C4BE
S5030002FA
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
101111111011
000011010000
011101111101
101110100001
000000001001
000011111000
100011100011
010110101101
111011111001
101110111110
001010101000
001100000001
000110000000
000111101011
100110100011
111011101010
000101010110
110001010111
001010001111
011101100010
110010110010
010100010101
000101001010
001010000010
001100100010
010011001111
101010111011
010111101000
001100010010
000010010001
111110111000
010011001000
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in1 -o test.out -vhdl_textio 16 12
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

