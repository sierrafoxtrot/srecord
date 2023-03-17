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

TEST_SUBJECT="input efinix"
. test_prelude.sh

cat > test.bit << 'fubar'
FB
0B
D0
70
7D
37
A1
7B
09
40
F8
50
E3
B8
AD
C5
F9
DE
BE
CB
A8
F2
01
C3
80
81
EB
41
A3
B9
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S1210000FB0BD0707D37A17B0940F850E3B8ADC5F9DEBECBA8F201C38081EB41A3B9E3
S5030001FB
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.bit -efinix -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
