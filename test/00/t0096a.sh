#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2005-2007 Peter Miller
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
        echo 'FAILED test of the CRLF functionality'
        exit 1
}

no_result()
{
        cd $here
        rm -rf $work
        echo 'NO RESULT for test of the CRLF functionality'
        exit 2
}

trap "no_result" 1 2 3 15

bin=$here/${1-.}/bin
mkdir $work
if test $? -ne 0; then no_result; fi
cd $work
if test $? -ne 0; then no_result; fi

cat > test.in << 'fubar'
S109000068656C6C6F0AD8
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok.srec << 'fubar'
S1230000533032323030303036383734373437303341324632463733373236353633364603
S12300203732363432453733364637353732363336353636364637323637363532453645C8
S123004036353734324631440D0A53313039303030303638363536433643364630414438E1
S11100600D0A533530333030303146420D0A2C
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.ok.srec -o test.ok -bin > LOG 2>&1
if test $? -ne 0; then cat LOG; no_result; fi

$bin/srec_cat test.in -o test.out -crlf > LOG 2>&1
if test $? -ne 0; then cat LOG; fail; fi

cmp test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
