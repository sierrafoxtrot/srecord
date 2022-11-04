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

TEST_SUBJECT="crop up to the last address"
. test_prelude.sh

cat > test.ok << 'fubar'
S00600004844521B
S306FFFFFFFE00FE
S5030001FB
fubar
if test $? -ne 0; then no_result; fi

srec_cat -generate 0xFFFFFFE0 0x100000000 -constant 0 \
    -crop 0xFFFFFFFE 0xFFFFFFFF -o test.out -header HDR
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# use the last address

cat > test.ok << 'fubar'
S00600004844521B
S306FFFFFFFF00FD
S5030001FB
fubar
if test $? -ne 0; then no_result; fi

srec_cat -generate 0xFFFFFFE0 0x100000000 -constant 0 \
    -crop 0xFFFFFFFF 0x100000000 -o test.out -header HDR
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
