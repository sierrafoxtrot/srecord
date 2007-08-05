#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2002, 2003, 2006, 2007 Peter Miller
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
        echo 'FAILED test of the os65v functionality'
        exit 1
}

no_result()
{
        cd $here
        rm -rf $work
        echo 'NO RESULT for test of the os65v functionality'
        exit 2
}

trap "no_result" 1 2 3 15

bin=$here/${1-.}/bin
mkdir $work
if test $? -ne 0; then no_result; fi
cd $work
if test $? -ne 0; then no_result; fi

cat > test.in << 'fubar'
S111000048656C6C6F2C20576F726C64210A7B
S9031000EC
fubar
if test $? -ne 0; then no_result; fi

# this is a binary format, so we have to encode the results,
# which is annoying because it obfuscates them.
cat > test.ok.mot << 'fubar'
S12300002E303030302F34380D36350D36430D36430D36460D32430D32300D35370D364653
S11900200D37320D36430D36340D32310D30410D2E313030304722
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.ok.mot -o test.ok -bin > LOG 2>&1
if test $? -ne 0; then cat LOG; no_result; fi

# This is the actual test
$bin/srec_cat test.in -o test.out -os65v > LOG 2>&1
if test $? -ne 0; then cat LOG; fail; fi

cmp test.ok test.out
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

$bin/srec_cat test.in -os -o test.out -header HDR > LOG 2>&1
if test $? -ne 0; then cat LOG; fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
