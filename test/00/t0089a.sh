#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2003, 2006, 2007 Peter Miller
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
#       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
#       02110-1301 USA.
#
# MANIFEST: Test the Verilog VMEM functionality
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
        echo 'FAILED test of the Verilog VMEM functionality'
        exit 1
}

no_result()
{
        cd $here
        rm -rf $work
        echo 'NO RESULT for test of the Verilog VMEM functionality'
        exit 2
}

trap "no_result" 1 2 3 15

bin=$here/${1-.}/bin
mkdir $work
if test $? -ne 0; then no_result; fi
cd $work
if test $? -ne 0; then no_result; fi

# ---------- Test Writing (32-bit) -----------------------------------------

cat > test.in << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
/* HDR */
@00000000 48656C6C 6F2C2057 6F726C64 210AFFFF
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in -fill 0xFF -within test.in -range-padding 4 \
    -o test.out -vmem
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- Test Writing (16-bit) -----------------------------------------

cat > test.ok << 'fubar'
/* hello */
@00000000 4865 6C6C 6F2C 2057 6F72 6C64 210A
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in -o test.out -vmem 16 -header hello
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- Test Writing (8-bit) ------------------------------------------

cat > test.ok << 'fubar'
/* HDR */
@00000000 48 65 6C
@00000003 6C 6F 2C
@00000006 20 57 6F
@00000009 72 6C 64
@0000000C 21 0A
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in -o test.out -vmem 8 -ll 20
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- Test Reading (32-bit) -----------------------------------------

cat > test.ok << 'fubar'
S00600004844521B
S113000048656C6C6F2C20576F726C64210AFFFF7B
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.in << 'fubar'
@00000000 48656C6C
@1 6F2C2057
@00000002
6F726C64
@0003 210AFFFF
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cmp test.ok test.in -vmem
if test $? -ne 0; then fail; fi

# ---------- Test Reading (16-bit) -----------------------------------------

cat > test.ok << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.in << 'fubar'
// comment
@0 4865 6C6C 6F2C 2057 6F72
@0005
6C64
210A
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cmp test.ok test.in -vmem
if test $? -ne 0; then fail; fi

# ---------- Test Reading (8-bit) ------------------------------------------

cat > test.ok << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.in << 'fubar'
@0 48 65 6C /* comment **/ 6C 6F 2C 20 57
@8 6F 72 6C 64 21 0A
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cmp test.ok test.in -vmem
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
