#!/bin/sh
#
#	srecord - manipulate eprom load files
#	Copyright (C) 1998 Peter Miller;
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
# MANIFEST: Test the -crop -over functionality
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
	echo 'FAILED test of the -crop -over functionality'
	exit 1
}

no_result()
{
	cd $here
	rm -rf $work
	echo 'NO RESULT for test of the -crop -over functionality'
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
S1230000436F70797269676874202843292031393938205065746572204D696C6C65723B94
S119002020416C6C207269676874732072657365727665642E0A24
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > mask << 'fubar'
S00600004844521B
S10700005858585898
S10700105858585888
S10700205858585878
S5030003F9
S9030030CC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S1230000436F70797269676874202843292031393938205065746572204D696C6C65723B94
S107002020416C6C9F
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in -crop -over mask -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
