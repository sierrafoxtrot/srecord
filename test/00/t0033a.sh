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
# MANIFEST: Test the wilson format functionality
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
	echo 'FAILED test of the wilson format functionality'
	exit 1
}

no_result()
{
	cd $here
	rm -rf $work
	echo 'NO RESULT for test of the wilson format functionality'
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
M%KSGC8*ZAOP[M2AVQU"5<MZ(I(,FABV3@8:OWHQ,9:()3#"+!K:'06RON#,`
M3:7>U4EB_-"/CU$5/B^ABY1$E.!T'^8JIRB65N#)5BUN-0.XE_^()H[9.\P(
1W5>=(>M]E0MDO[*,5@AL'U\`
`
end
fubar
if test $? -ne 0; then no_result; fi

uudecode test.in.uue
if test $? -ne 0; then no_result; fi

cat > test.ok.uue << 'fubar'
begin 644 test.ok
M(V5`0$!`5CL\Y\W".SK&_'L[-6BV/#>0U;(]/L@Z-,-FQFW3P<8Z/ST^S(RE
M.C([/@HC94!`0&!)C'#+1CLVQX&L.C\[.'-`C3HU/3X]-8FB_#TPS\^157YO
M.C'+U(34NPHC94!`0(#@M%_F:CHW:-:6X#PYEFVN=4,[.-?_R&;./3E[/#Q(
G/3V7W6'KO3L^"B-00$!`H-5+I#L_.S+,EDBL7Y^&"B=%0$!`0/H*
`
end
fubar
if test $? -ne 0; then no_result; fi

uudecode test.ok.uue
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in -bin -o test.out -wilson
if test $? -ne 0; then fail; fi

cmp test.ok test.out
if test $? -ne 0; then fail; fi

#
# Test input
#
mv test.out test.in
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S123000016BCE78D82BA86FC3BB52876C7509572DE88A48326862D938186AFDE8C4C65A2C0
S1230020094C308B06B687416CAFB833004DA5DED54962FCD08F8F51153E2FA18B9444947D
S1230040E0741FE62AA7289656E0C9562D6E3503B897FF88268ED93BCC08DD579D21EB7DC0
S10E0060950B64BFB28C56086C1F5F48
S5030004F8
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in -wilson -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
