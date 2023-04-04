#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2002, 2003, 2006-2008 Peter Miller
#
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 3 of the License, or
#       (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with this program. If not, see
#       <http://www.gnu.org/licenses/>.
#

TEST_SUBJECT="os65a format"
. test_prelude.sh

cat > test.in << 'fubar'
S111000048656C6C6F2C20576F726C64210A7B
S9031000EC
fubar
if test $? -ne 0; then no_result; fi

printf "RL0000\n48656C6C6F2C20576F726C64210A\nR" > test.ok

if test $? -ne 0; then no_result; fi

# This is the actual test
srec_cat test.in -o test.out -os65a -eol=nl -disable=esa > LOG 2>&1
if test $? -ne 0; then
    cat LOG
    fail
fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

printf "RL0000\n48656C6C6F2C20576F726C64210A\nRL012E1000RG" > test.ok

if test $? -ne 0; then no_result; fi

# This is the actual test
srec_cat test.in -o test.out -os65a -eol=nl > LOG 2>&1
if test $? -ne 0; then
    cat LOG
    fail
fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

mv test.out test.in
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
S9031000EC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -os65a -o test.out -header HDR > LOG 2>&1
if test $? -ne 0; then
    cat LOG
    fail
fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
