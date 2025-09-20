#!/bin/sh
#
#       srecord - The "srecord" program.
#       Copyright (C) 2007, 2008, 2011 Peter Miller
#
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 3 of the License, or
#       (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with this program. If not, see
#       <http://www.gnu.org/licenses/>.
#

TEST_SUBJECT="-c-array -output-word 64"
. test_prelude.sh

cat > test.in << 'fubar'
S00600004844521B
S111000748656C6C6F2C20576F726C64210A74
S111002048656C6C6F2C20576F726C64210A5B
S111004248656C6C6F2C20576F726C64210A39
S5030003F9
S9030007F5
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
/* HDR */
#include <stdint.h>
const uint64_t eprom[] =
{
0x48FFFFFFFFFFFFFF, 0x6F57202C6F6C6C65, 0xFFFFFF0A21646C72,
0x57202C6F6C6C6548, 0xFFFF0A21646C726F, 0x2C6F6C6C6548FFFF,
0x0A21646C726F5720,
};

const unsigned long eprom_address[] =
{
0x00000000, 0x00000020, 0x00000040,
};
const unsigned long eprom_word_address[] =
{
0x00000000, 0x00000004, 0x00000008,
};
const unsigned long eprom_length_of_sections[] =
{
0x00000003, 0x00000002, 0x00000002,
};
const unsigned long eprom_sections    = 0x00000003;
const unsigned long eprom_termination = 0x00000007;
const unsigned long eprom_start       = 0x00000000;
const unsigned long eprom_finish      = 0x00000050;
const unsigned long eprom_length      = 0x00000050;

#define EPROM_TERMINATION 0x00000007
#define EPROM_START       0x00000000
#define EPROM_FINISH      0x00000050
#define EPROM_LENGTH      0x00000050
#define EPROM_SECTIONS    0x00000003
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok.h << 'fubar'
#ifndef TEST_H
#define TEST_H
#include <stdint.h>

extern const unsigned long eprom_termination;
extern const unsigned long eprom_start;
extern const unsigned long eprom_finish;
extern const unsigned long eprom_length;
extern const unsigned long eprom_sections;
extern const uint64_t eprom[];
extern const unsigned long eprom_address[];
extern const unsigned long eprom_word_address[];
extern const unsigned long eprom_length_of_sections[];

#endif /* TEST_H */
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0xFF -within test.in -range-padding=8 \
    -o test.out -c-array -section-style -ow 8 -include
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

diff test.ok.h test.h
if test $? -ne 0; then fail; fi

# ---------- one more time, with decimal uint64_t words -----------------------

cat > test.ok << 'fubar'
/* HDR */
#include <stdint.h>
const uint64_t eprom[] =
{
5260204364768739327, 8022916636403854437, 18446743017707826290,
6278066737626506568, 18446473737267868271, 3201897072895262719,
729975031549876000,
};

const unsigned long eprom_address[] =
{
0, 32, 64,
};
const unsigned long eprom_word_address[] =
{
0, 4, 8,
};
const unsigned long eprom_length_of_sections[] =
{
3, 2, 2,
};
const unsigned long eprom_sections    = 3;
const unsigned long eprom_termination = 7;
const unsigned long eprom_start       = 0;
const unsigned long eprom_finish      = 80;
const unsigned long eprom_length      = 80;

#define EPROM_TERMINATION 7
#define EPROM_START       0
#define EPROM_FINISH      80
#define EPROM_LENGTH      80
#define EPROM_SECTIONS    3
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0xFF -within test.in -range-padding=8 \
    -o test.out -c-array -section-style -ow 8 -dec-style
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- --output-word 8 is equivalent to -output-word 64 -----------------

srec_cat test.in -fill 0xFF -within test.in -range-padding=8 \
    -o test.out -c-array -section-style -ow 64 -dec-style
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- one more time, with byte swapped input ---------------------------

cat > test.ok << 'fubar'
/* 00000007: HDR */
#include <stdint.h>
const uint64_t eprom[] =
{
0xFFFFFFFFFFFFFF48, 0x656C6C6F2C20576F, 0x726C64210AFFFFFF,
0x48656C6C6F2C2057, 0x6F726C64210AFFFF, 0xFFFF48656C6C6F2C,
0x20576F726C64210A,
};

const unsigned long eprom_address[] =
{
0x00000000, 0x00000020, 0x00000040,
};
const unsigned long eprom_word_address[] =
{
0x00000000, 0x00000004, 0x00000008,
};
const unsigned long eprom_length_of_sections[] =
{
0x00000003, 0x00000002, 0x00000002,
};
const unsigned long eprom_sections    = 0x00000003;
const unsigned long eprom_termination = 0x00000000;
const unsigned long eprom_start       = 0x00000000;
const unsigned long eprom_finish      = 0x00000050;
const unsigned long eprom_length      = 0x00000050;

#define EPROM_TERMINATION 0x00000000
#define EPROM_START       0x00000000
#define EPROM_FINISH      0x00000050
#define EPROM_LENGTH      0x00000050
#define EPROM_SECTIONS    0x00000003
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0xFF -within test.in -range-padding=8 -byte-swap 8 \
    -o test.out -c-array -section-style -ow 8
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- Fatal alignment error --------------------------------------------
cat > test.in << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S123000C000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1FE0
S123002C202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3FC0
S5030002FA
S903000CF0
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
srec_cat: test.out: 5: The C-Array (uint64_t) output format uses 8-byte
    alignment, but unaligned data is present. Use a "--fill 0xNN --within
    <input> --range-padding 8" filter to fix this problem.
fubar

srec_cat test.in  -o test.out -c-array -ow 8  > LOG 2>&1
if test $? -ne 1; then
    cat LOG
    fail
fi
diff test.ok LOG
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
