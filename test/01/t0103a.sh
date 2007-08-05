#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2006, 2007 Peter Miller
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
        echo 'FAILED test of the cmd line parsing functionality'
        exit 1
}

no_result()
{
        cd $here
        rm -rf $work
        echo 'NO RESULT for test of the cmd line parsing functionality'
        exit 2
}

trap "no_result" 1 2 3 15

bin=$here/${1-.}/bin
mkdir $work
if test $? -ne 0; then no_result; fi
cd $work
if test $? -ne 0; then no_result; fi

cat > test.in << 'fubar'
S00E00004170706C696361746D6F7473
S12327E0F5A639C0139B32163E25FD5CF1040C7E4EA87EB9D0F5C60DEB7CCC9EDAF72D2C50
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00E00004170706C696361746D6F7473
S12327E0F5A639C0139B32163E25FD5CF1040C7E4EA87EB9D0F5C60DEB7CCC9EDAF72D2C50
S10528002D465F
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in -be_checksum_neg '(' -max test.in ')' 2 2 > test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
