#!/bin/sh
#
#	srecord - manipulate eprom load files
#	Copyright (C) 2001, 2003, 2006 Peter Miller;
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
# MANIFEST: Test the PFC functionality
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
	echo 'FAILED test of the PFC functionality'
	exit 1
}

no_result()
{
	cd $here
	rm -rf $work
	echo 'NO RESULT for test of the PFC functionality'
	exit 2
}

trap "no_result" 1 2 3 15

bin=$here/${1-.}/bin
mkdir $work
if test $? -ne 0; then no_result; fi
cd $work
if test $? -ne 0; then no_result; fi

#---------------------------------------------------------------------------
#
# Test reading the format
#
cat > test.in << 'fubar'
$kL&@h%%,:,B.\?00EPuX0K3rO0JI))
$;UPR'%%,:<Hn&FCG:at<GVF(;G9wIw
$7FD1p%%,:LHmy:>GTV%/KJ7@GE[kYz
$B[6\;%%,:\KIn?GFWY/qKI1G5:;-_e
$%%%%%
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S123B000576F77212044696420796F75207265616C6C7920676F207468726F7567682061DF
S120B0206C6C20746861742074726F75626C6520746F207265616420746869733F73
S5030002FA
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in -fpc -o test.out -header HDR
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#---------------------------------------------------------------------------
#
# Test writing the format
#
cat > test.ok << 'fubar'
$mbw6)%%,:,B.\?00EPuX0K3rO0JI))Hn&FCG:at<GVF(;G9wIw
$K%6Re%%,:LHmy:>GTV%/KJ7@GE[kYzKIn?GFWY/qKI1G5:;-_e
$%%%%%
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in -fpc -o test.out -fpc
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
