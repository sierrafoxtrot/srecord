#!/bin/sh
#
#       srecord - The "srecord" program.
#       Copyright (C) 2007 Peter Miller
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
        echo 'FAILED test of the CRC16 functionality'
        exit 1
}

no_result()
{
        cd $here
        rm -rf $work
        echo 'NO RESULT for test of the CRC16 functionality'
        exit 2
}

trap "no_result" 1 2 3 15

bin=$here/${1-.}/bin
mkdir $work
if test $? -ne 0; then no_result; fi
cd $work
if test $? -ne 0; then no_result; fi

#
# Build the CRC16 test vectors
#
cp /dev/null zero.length.file
test $? -eq 0 || no_result
echo A | dd bs=1 count=1 2>/dev/null > single.a.file
test $? -eq 0 || no_result
echo 123456789 | dd bs=9 count=1 2>/dev/null > nine.digits.file
test $? -eq 0 || no_result

echo "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" |
dd bs=64 count=1 2>/dev/null > upper-case-a.64.file
test $? -eq 0 || no_result
cat upper-case-a.64.file upper-case-a.64.file upper-case-a.64.file \
        upper-case-a.64.file > upper-case-a.256.file
test $? -eq 0 || no_result

#
# now we run the test vectors with and without augmentation
#
$bin/test_crc16 -a < zero.length.file > test.out
test $? -eq 0 || fail
echo >> test.out
test $? -eq 0 || no_result

$bin/test_crc16 -a < single.a.file >> test.out
test $? -eq 0 || fail
echo >> test.out
test $? -eq 0 || no_result

$bin/test_crc16 -a < nine.digits.file >> test.out
test $? -eq 0 || fail
echo >> test.out
test $? -eq 0 || no_result

$bin/test_crc16 -a < upper-case-a.256.file >> test.out
test $? -eq 0 || fail

cat > test.ok << 'fubar'
0xFFFF

0xE1B1

0xA69D

0xA4CE
fubar
if test $? -ne 0; then no_result; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim:ts=8:sw=4:et
