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

TEST_SUBJECT="-c-array -output-word 16"
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
const uint16_t eprom[] =
{
0x48FF, 0x6C65, 0x6F6C, 0x202C, 0x6F57, 0x6C72, 0x2164, 0xFF0A, 0x6548,
0x6C6C, 0x2C6F, 0x5720, 0x726F, 0x646C, 0x0A21, 0x6548, 0x6C6C, 0x2C6F,
0x5720, 0x726F, 0x646C, 0x0A21,
};

const unsigned long eprom_address[] =
{
0x00000006, 0x00000020, 0x00000042,
};
const unsigned long eprom_word_address[] =
{
0x00000003, 0x00000010, 0x00000021,
};
const unsigned long eprom_length_of_sections[] =
{
0x00000008, 0x00000007, 0x00000007,
};
const unsigned long eprom_sections    = 0x00000003;
const unsigned long eprom_termination = 0x00000007;
const unsigned long eprom_start       = 0x00000006;
const unsigned long eprom_finish      = 0x00000050;
const unsigned long eprom_length      = 0x0000004A;

#define EPROM_TERMINATION 0x00000007
#define EPROM_START       0x00000006
#define EPROM_FINISH      0x00000050
#define EPROM_LENGTH      0x0000004A
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
extern const uint16_t eprom[];
extern const unsigned long eprom_address[];
extern const unsigned long eprom_word_address[];
extern const unsigned long eprom_length_of_sections[];

#endif /* TEST_H */
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0xFF -within test.in -range-padding=2 \
    -o test.out -c-array -section-style -ow 2 -include
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

diff test.ok.h test.h
if test $? -ne 0; then fail; fi

# ---------- one more time, with decimal uint16_t words -----------------------

cat > test.ok << 'fubar'
/* HDR */
#include <stdint.h>
const uint16_t eprom[] =
{
18687, 27749, 28524, 8236, 28503, 27762, 8548, 65290, 25928, 27756, 11375,
22304, 29295, 25708, 2593, 25928, 27756, 11375, 22304, 29295, 25708, 2593,
};

const unsigned long eprom_address[] =
{
6, 32, 66,
};
const unsigned long eprom_word_address[] =
{
3, 16, 33,
};
const unsigned long eprom_length_of_sections[] =
{
8, 7, 7,
};
const unsigned long eprom_sections    = 3;
const unsigned long eprom_termination = 7;
const unsigned long eprom_start       = 6;
const unsigned long eprom_finish      = 80;
const unsigned long eprom_length      = 74;

#define EPROM_TERMINATION 7
#define EPROM_START       6
#define EPROM_FINISH      80
#define EPROM_LENGTH      74
#define EPROM_SECTIONS    3
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0xFF -within test.in -range-padding=2 \
    -o test.out -c-array -section-style -ow 2 -dec-style
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- --output-word 2 is equivalent to -output-word 16 -----------------

srec_cat test.in -fill 0xFF -within test.in -range-padding=2 \
    -o test.out -c-array -section-style -ow 16 -dec-style
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- one more time, with byte swapped input ---------------------------

cat > test.ok << 'fubar'
/* 00000001: HDR */
#include <stdint.h>
const uint16_t eprom[] =
{
0xFF48, 0x656C, 0x6C6F, 0x2C20, 0x576F, 0x726C, 0x6421, 0x0AFF, 0x4865,
0x6C6C, 0x6F2C, 0x2057, 0x6F72, 0x6C64, 0x210A, 0x4865, 0x6C6C, 0x6F2C,
0x2057, 0x6F72, 0x6C64, 0x210A,
};

const unsigned long eprom_address[] =
{
0x00000006, 0x00000020, 0x00000042,
};
const unsigned long eprom_word_address[] =
{
0x00000003, 0x00000010, 0x00000021,
};
const unsigned long eprom_length_of_sections[] =
{
0x00000008, 0x00000007, 0x00000007,
};
const unsigned long eprom_sections    = 0x00000003;
const unsigned long eprom_termination = 0x00000006;
const unsigned long eprom_start       = 0x00000006;
const unsigned long eprom_finish      = 0x00000050;
const unsigned long eprom_length      = 0x0000004A;

#define EPROM_TERMINATION 0x00000006
#define EPROM_START       0x00000006
#define EPROM_FINISH      0x00000050
#define EPROM_LENGTH      0x0000004A
#define EPROM_SECTIONS    0x00000003
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0xFF -within test.in -range-padding=2 -byte-swap 2 \
    -o test.out -c-array -section-style -ow 2
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- Fatal alignment error --------------------------------------------
cat > test.in << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S1230001000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1FEB
S5030001FB
S9030001FB
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
srec_cat: test.out: 5: The C-Array (uint16_t) output format uses 16-bit data,
    but unaligned data is present. Use a "--fill 0xNN --within <input>
    --range-padding 2" filter to fix this problem.
fubar

srec_cat test.in  -o test.out -c-array -ow 2  > LOG 2>&1
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
