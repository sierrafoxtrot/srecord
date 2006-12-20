#!/bin/sh
#
#	srecord - manipulate eprom load files
#	Copyright (C) 2005, 2006 Peter Miller
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
# MANIFEST: Test the vmem functionality
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
	echo 'FAILED test of the vmem functionality'
	exit 1
}

no_result()
{
	cd $here
	rm -rf $work
	echo 'NO RESULT for test of the vmem functionality'
	exit 2
}

trap "no_result" 1 2 3 15

bin=$here/${1-.}/bin
mkdir $work
if test $? -ne 0; then no_result; fi
cd $work
if test $? -ne 0; then no_result; fi

cat > test.in << 'fubar'
:1000A0000302050C3000800C010C2600E307560A01
:1000B0000302020C2600030210022700E3075E0A77
:0600C000F002530A500A91
:0203FE00500AA3
:00000001FF
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
/* http://srecord.sourceforge.net/ */
@00000050 0203 0C05 0030 0C80 0C01 0026 07E3 0A56
@00000058 0203 0C02 0026 0203 0210 0027 07E3 0A5E
@00000060 02F0 0A53 0A50
@000001FF 0A50
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in -intel -byteswap -o test.out -vmem 16 > LOG 2>&1
if test $? -ne 0
then
    cat LOG
    fail
fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
