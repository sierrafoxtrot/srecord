#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2004, 2006, 2007 Peter Miller
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
# MANIFEST: Test the stewie functionality
#
here=`pwd`
if test $? -ne 0 ; then exit 2; fi
work=${TMP_DIR-/tmp}/$$

pass()
{
        cd $here
        rm -rf $work
        echo PASSED
        exit 0
}

fail()
{
        cd $here
        rm -rf $work
        echo 'FAILED test of the stewie functionality'
        exit 1
}

no_result()
{
        cd $here
        rm -rf $work
        echo 'NO RESULT for test of the stewie functionality'
        exit 2
}

trap "no_result" 1 2 3 15

bin=$here/${1-.}/bin
mkdir $work
if test $? -ne 0; then no_result; fi
cd $work
if test $? -ne 0; then no_result; fi

cat > test.in << 'fubar'
S00600004844521B
S110000048656C6C6F2C20576F726C640A9D
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok.srec << 'fubar'
S0030000FC
S11C000053303033533110000048656C6C6F2C20576F726C640A9D5338EF
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.ok.srec -o test.ok -bin
if test $? -ne 0; then fail; fi

#
# Read Motorola format and write Stewie format.
#
$bin/srec_cat test.in -o test.out -stewie
if test $? -ne 0; then fail; fi

cmp test.ok test.out
if test $? -ne 0; then fail; fi

#
# Read Stewie format and write Motorola format.
#
$bin/srec_cat test.out -stewie -o test.out2 -header HDR
if test $? -ne 0; then fail; fi

cmp test.in test.out2
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
