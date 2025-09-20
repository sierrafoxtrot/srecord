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

TEST_SUBJECT="-c-array -output-word 32"
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
const uint32_t eprom[] =
{
0x48FFFFFF, 0x6F6C6C65, 0x6F57202C, 0x21646C72, 0xFFFFFF0A, 0x6C6C6548,
0x57202C6F, 0x646C726F, 0xFFFF0A21, 0x6548FFFF, 0x2C6F6C6C, 0x726F5720,
0x0A21646C,
};

const unsigned long eprom_address[] =
{
0x00000004, 0x00000020, 0x00000040,
};
const unsigned long eprom_word_address[] =
{
0x00000001, 0x00000008, 0x00000010,
};
const unsigned long eprom_length_of_sections[] =
{
0x00000005, 0x00000004, 0x00000004,
};
const unsigned long eprom_sections    = 0x00000003;
const unsigned long eprom_termination = 0x00000007;
const unsigned long eprom_start       = 0x00000004;
const unsigned long eprom_finish      = 0x00000050;
const unsigned long eprom_length      = 0x0000004C;

#define EPROM_TERMINATION 0x00000007
#define EPROM_START       0x00000004
#define EPROM_FINISH      0x00000050
#define EPROM_LENGTH      0x0000004C
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
extern const uint32_t eprom[];
extern const unsigned long eprom_address[];
extern const unsigned long eprom_word_address[];
extern const unsigned long eprom_length_of_sections[];

#endif /* TEST_H */
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0xFF -within test.in -range-padding=4 \
    -o test.out -c-array -section-style -ow 4 -include
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

diff test.ok.h test.h
if test $? -ne 0; then fail; fi

# ---------- one more time, with decimal uint32_t words -----------------------

cat > test.ok << 'fubar'
/* HDR */
#include <stdint.h>
const uint32_t eprom[] =
{
1224736767, 1869376613, 1867980844, 560229490, 4294967050, 1819043144,
1461726319, 1684828783, 4294904353, 1699282943, 745499756, 1919899424,
169960556,
};

const unsigned long eprom_address[] =
{
4, 32, 64,
};
const unsigned long eprom_word_address[] =
{
1, 8, 16,
};
const unsigned long eprom_length_of_sections[] =
{
5, 4, 4,
};
const unsigned long eprom_sections    = 3;
const unsigned long eprom_termination = 7;
const unsigned long eprom_start       = 4;
const unsigned long eprom_finish      = 80;
const unsigned long eprom_length      = 76;

#define EPROM_TERMINATION 7
#define EPROM_START       4
#define EPROM_FINISH      80
#define EPROM_LENGTH      76
#define EPROM_SECTIONS    3
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0xFF -within test.in -range-padding=4 \
    -o test.out -c-array -section-style -ow 4 -dec-style
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- --output-word 4 is equivalent to -output-word 32 -----------------

srec_cat test.in -fill 0xFF -within test.in -range-padding=4 \
    -o test.out -c-array -section-style -ow 32 -dec-style
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- one more time, with byte swapped input ---------------------------

cat > test.ok << 'fubar'
/* 00000003: HDR */
#include <stdint.h>
const uint32_t eprom[] =
{
0xFFFFFF48, 0x656C6C6F, 0x2C20576F, 0x726C6421, 0x0AFFFFFF, 0x48656C6C,
0x6F2C2057, 0x6F726C64, 0x210AFFFF, 0xFFFF4865, 0x6C6C6F2C, 0x20576F72,
0x6C64210A,
};

const unsigned long eprom_address[] =
{
0x00000004, 0x00000020, 0x00000040,
};
const unsigned long eprom_word_address[] =
{
0x00000001, 0x00000008, 0x00000010,
};
const unsigned long eprom_length_of_sections[] =
{
0x00000005, 0x00000004, 0x00000004,
};
const unsigned long eprom_sections    = 0x00000003;
const unsigned long eprom_termination = 0x00000004;
const unsigned long eprom_start       = 0x00000004;
const unsigned long eprom_finish      = 0x00000050;
const unsigned long eprom_length      = 0x0000004C;

#define EPROM_TERMINATION 0x00000004
#define EPROM_START       0x00000004
#define EPROM_FINISH      0x00000050
#define EPROM_LENGTH      0x0000004C
#define EPROM_SECTIONS    0x00000003
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0xFF -within test.in -range-padding=4 -byte-swap 4 \
    -o test.out -c-array -section-style -ow 4
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- Fatal alignment error --------------------------------------------
cat > test.in << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S1230002000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1FEA
S5030001FB
S9030001FB
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
srec_cat: test.out: 5: The C-Array (uint32_t) output format uses 32-bit data,
    but unaligned data is present. Use a "--fill 0xNN --within <input>
    --range-padding 4" filter to fix this problem.
fubar

srec_cat test.in  -o test.out -c-array -ow 4  > LOG 2>&1
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
