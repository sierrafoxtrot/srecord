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

TEST_SUBJECT="input hp64000"
. test_prelude

cat > test.in1 << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S123000082043848502056312E3720200000000000500007001000086BF000000009000463
S11500200A44000170077871000700020A54000103F5BB
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

#create intermediate file, actual hp64k format
srec_cat test.in1 -moto -out test.in2 -bin
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
Format: HP64000 Absolute
Header: "8HP V1.7  "
Execution Start Address: 00006BF0
Data:   010A44 - 010A47
        010A54 - 010A55
fubar
if test $? -ne 0; then no_result; fi

srec_info test.in2 -hp64 > test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
