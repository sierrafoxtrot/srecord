#!/bin/sh
#
#	srecord - manipulate eprom load files
#	Copyright (C) 2000 Peter Miller;
#	All rights reserved.
#
#	This program is free software; you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation; either version 2 of the License, or
#	(at your option) any later version.
#
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with this program; if not, write to the Free Software
#	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
#
# MANIFEST: Test the srec_cmp --verbose functionality
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
	echo 'FAILED test of the srec_cmp --verbose functionality'
	exit 1
}

no_result()
{
	cd $here
	rm -rf $work
	echo 'NO RESULT for test of the srec_cmp --verbose functionality'
	exit 2
}

trap "no_result" 1 2 3 15

bin=$here/${1-.}/bin
mkdir $work
if test $? -ne 0; then no_result; fi
cd $work
if test $? -ne 0; then no_result; fi

cat > test.one << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C642E0A6E
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.two << 'fubar'
S00600004844521B
S111000068656C6C6F5F5F686F726C642E0ACB
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
Different:	(0x1, 0x2, 0x4 - 0xd)
Left only:	0xe
Right only:	0x0
Start address 1 not equal to 0.
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cmp -verbose test.one -offset 1 test.two > test.out 2>&1
if test $? -ne 2; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
