#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2014 Markus Heidelberg
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

TEST_SUBJECT="random fill near end of address space"
. test_prelude

cat > test.ok << 'fubar'
Format: Motorola S-Record
Header: "HDR"
Execution Start Address: 00000000
Data:   00000000 - 00000000
        FFFFFFFE - FFFFFFFE
fubar
if test $? -ne 0; then no_result; fi

srec_cat -generate 0 1 -constant 0 -random-fill 0xFFFFFFFE 0xFFFFFFFF \
        -o test.srec -header HDR -start-addr 0
if test $? -ne 0; then fail; fi

srec_info test.srec > test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# use the last address
# -> buggy srec_info output is a different issue

cat > test.ok << 'fubar'
Format: Motorola S-Record
Header: "HDR"
Execution Start Address: 00000000
Data:   0000 - 0000
        FFFFFFFF - FFFFFFFFFFFFFFFF
fubar
if test $? -ne 0; then no_result; fi

srec_cat -generate 0 1 -constant 0 -random-fill 0xFFFFFFFF 0x100000000 \
        -o test.srec -header HDR -start-addr 0
if test $? -ne 0; then fail; fi

srec_info test.srec > test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
