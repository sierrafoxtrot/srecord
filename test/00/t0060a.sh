#!/bin/sh
#
#	srecord - manipulate eprom load files
#	Copyright (C) 2001 Peter Miller;
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
# MANIFEST: Test the memory chunk iterator functionality
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
	echo 'FAILED test of the memory chunk iterator functionality'
	exit 1
}

no_result()
{
	cd $here
	rm -rf $work
	echo 'NO RESULT for test of the memory chunk iterator functionality'
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
MU&Z]WF>18O<HR70CV+BLT0/CXG76.#L"H%V_HOGT=;!"'%C8QHM%%B\HC+_P
M?^LBX815!T<'3Y/['#+>`!:=/VGKE1@^"OSWQ9QNSSX4)69*J3),0_]^Z:.J
MX1I&8BF(X;'0"P:)E:(C0#%-4V/7M8@]1D-BJ9QGS.*Z`U[/];T-GZC05?`,
MH$:A=:.J!M^C(R[RS<L]U,Q(%(.)%/2#1A+@,&0)P5.G&)+*KFN^*-E/E&DT
MFT86-JSHB1\$?)M=N?8J_3CN_!)ZV27R%6)TO@1#D`V^3G2^7MXX0]-=WI>`
MHM*+LB]GIJPV39E[LL:`]#HW<$5(R=`#GFB>8\KCWRQKV;&&-&/@-9KZY3O!
MBSJN8S7[T5YGH/1%8#+M2XNC,F&)/:\6B$&6%+I_&`5+:JEQ30'#9[OEM"],
M`F:`/>:NA]:E:D1OFR;0602*S>9#,@H1'9]:%?)Y[MV,^\>7:(50QAK<>./N
M6-]+^S=2DP[:1-JCK3;2UJ&@4XIN<G2N:RV=A=1R.C[3E&`E'*]2Y@4IIN<\
M@G(_NA!?]BUY7S>U0=Q#J^9W.&?",N5:_T0)G1)*B]J^*+6`_8OBJR'5URM]
MXVQB#1)'Y*BP,&?^76-O*O&@M8Q%#D]W(?[B*1OM^J97]K^<S"N?QQG?"[F_
1>I+'+?FXE?1"$W.'PR%QT$D`
`
end
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
Different:	0x40 - 0x43
fubar
if test $? -ne 0; then no_result; fi

uudecode test.in.uue
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in -bin -exclude 0x40 0x44 -becrc32 0x40 -o test.out -bin
if test $? -ne 0; then fail; fi

$bin/srec_cmp -v test.in -bin test.out -bin > log
if test $? -ne 2; then fail; fi

diff test.ok log
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
