#!/bin/sh
#
#       srecord - The "srecord" program.
#       Copyright (C) 2007 Peter Miller
#
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 2 of the License, or
#       (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with this program; if not, write to the Free Software
#       Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
#
# MANIFEST: Test the interval arithmetic functionality
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
        echo 'FAILED test of the interval arithmetic functionality'
        exit 1
}

no_result()
{
        cd $here
        rm -rf $work
        echo 'NO RESULT for test of the interval arithmetic functionality'
        exit 2
}

trap "no_result" 1 2 3 15

bin=$here/${1-.}/bin
mkdir $work
if test $? -ne 0; then no_result; fi
cd $work
if test $? -ne 0; then no_result; fi

# ---------- set difference ------------------------------------------------

cat > test.ok << 'fubar'
Format: Motorola S-Record
Header: "HDR"
Start:  00000000
Data:   0000 - 00FF
        0E00 - 0FFF
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat -generate { 0 0x1000 -diff 0x100 0xE00 } -random \
    -o test.srec -header HDR -start-addr 0
if test $? -ne 0; then fail; fi

$bin/srec_info test.srec > test.out
if test $? -ne 0; then no_result; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- set intersection ----------------------------------------------

cat > test.ok << 'fubar'
Format: Motorola S-Record
Header: "HDR"
Start:  00000000
Data:   0100 - 0FFF
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat -generate { 0 0x1000 -intersect 0x100 0xE000 } -random \
    -o test.srec -header HDR -start-addr 0
if test $? -ne 0; then fail; fi

$bin/srec_info test.srec > test.out
if test $? -ne 0; then no_result; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- set union -----------------------------------------------------

cat > test.ok << 'fubar'
Format: Motorola S-Record
Header: "HDR"
Start:  00000000
Data:   0080 - 00C7
        0190 - 0257
        0320 - 03E7
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat -generate { { 0 200 400 600 800 1000 } -intersect 0x80 0xE000 } \
    -random \
    -o test.srec -header HDR -start-addr 0
if test $? -ne 0; then fail; fi

$bin/srec_info test.srec > test.out
if test $? -ne 0; then no_result; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim:ts=8:sw=4:et
