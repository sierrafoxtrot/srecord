#!/bin/sh
#
#	srecord - manipulate eprom load files
#	Copyright (C) 2000, 2003 Peter Miller;
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
# MANIFEST: Test the ascii-hex format functionality
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
	echo 'FAILED test of the ascii-hex format functionality'
	exit 1
}

no_result()
{
	cd $here
	rm -rf $work
	echo 'NO RESULT for test of the ascii-hex format functionality'
	exit 2
}

trap "no_result" 1 2 3 15

bin=$here/${1-.}/bin
mkdir $work
if test $? -ne 0; then no_result; fi
cd $work
if test $? -ne 0; then no_result; fi

# --------------------------------------------------------------------------
#
# test writing the format
#
cat > test.in << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok.srec << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S123000002202441303132332C0A3438203635203643203643203646203243203230203528
S12100203720364620373220364320363420323120304120030A2453303437332C0A83
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.ok.srec -o test.ok -bin
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in -offset 0x123 -o test.out -asc-hex
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# --------------------------------------------------------------------------
#
# test reading the format
#
cat > test.in.uue << 'fubar'
S00600004844521B
S123000002203438203635203643203643203646203243203230202441303031332C203514
S11900203720364620373220364320363420323120304120030A06
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in.uue -o test.in -bin
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S10A000048656C6C6F2C20B5
S10A0013576F726C64210AAF
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in -asc-hex -o test.out -header HDR
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
