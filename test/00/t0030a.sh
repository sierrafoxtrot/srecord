#!/bin/sh
#
#	srecord - manipulate eprom load files
#	Copyright (C) 1999 Peter Miller;
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
#	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# MANIFEST: Test the WindowsNT binary functionality
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
	echo 'FAILED test of the WindowsNT binary functionality'
	exit 1
}

no_result()
{
	cd $here
	rm -rf $work
	echo 'NO RESULT for test of the WindowsNT binary functionality'
	exit 2
}

trap "no_result" 1 2 3 15

bin=$here/${1-.}/bin
mkdir $work
if test $? -ne 0; then no_result; fi
cd $work
if test $? -ne 0; then no_result; fi

cat > test.in.uue << 'fubar'
begin 644 test.in
75&AI<R!I<PIA(&YA<W1Y#0IT97-T+@T`
`
end
fubar
if test $? -ne 0; then no_result; fi

cat > test.in << 'fubar'
S00600004844521B
S11A0000546869732069730A61206E617374790D0A746573742E0D85
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in -o test.int -bin
if test $? -ne 0; then fail; fi

$bin/srec_cat test.int -bin -o test.out
if test $? -ne 0; then fail; fi

diff test.in test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
