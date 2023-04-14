#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2023 Daniel Anselmi
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

TEST_SUBJECT="input gowin"
. test_prelude.sh

cat > test.bit << 'fubar'
//comment line
01001000
0110010101101100
01100101
0110111100101100
00100000
010101110110111101110010
0110110001100100
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S10F000048656C656F2C20576F726C64AF
S5030001FB
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.bit -gowin -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
