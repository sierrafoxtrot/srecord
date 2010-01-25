//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2002, 2006-2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//
//
// Implemented from scratch from
// "A painless guide to CRC error detection algorithms"
// http://www.repairfaq.org/filipg/LINK/F_crc_v3.html
//
// See also http://www.joegeluso.com/software/articles/ccitt.htm
//
// See test/01/t0150a.sh for test vectors.
//

#include <srecord/bitrev.h>
#include <srecord/crc16.h>

//
// Use a seed of 0xFFFF when augmenting manually (i.e. augmenting by 16
// zero bits by processing two zero bytes at the end of the data), but a
// seed of 0x1D0F when the augmenting is done by shifting where the XOR
// appears in the updcrc function.
//
// The 0x1D0F value is calculated by using the manual augmentation
// updcrc function:
//     ubdcrc(0, updcrc(0, 0xFFFF))
//
static unsigned short const ccitt_seed = 0xFFFF;
static unsigned short const broken_seed = 0x84CF;
static unsigned short const xmodem_seed = 0;


void
srecord::crc16::calculate_table()
{
    if (polynomial == 0)
        polynomial = polynomial_ccitt;
    if (bitdir == bit_direction_most_to_least)
    {
        for (unsigned b = 0; b < 256; ++b)
        {
            unsigned short v = b << 8;
            for (unsigned j = 0; j < 8; ++j)
                v = (v & 0x8000) ? ((v << 1) ^ polynomial) : (v << 1);
            table[b] = v;
        }
    }
    else
    {
        polynomial = bitrev16(polynomial);
        for (unsigned b = 0; b < 256; ++b)
        {
            unsigned short v = b;
            for (unsigned j = 0; j < 8; ++j)
                v = (v & 1) ? ((v >> 1) ^ polynomial) : (v >> 1);
            table[b] = v;
        }
    }
}


static int
state_from_seed_mode(srecord::crc16::seed_mode_t seed_mode)
{
    switch (seed_mode)
    {
    case srecord::crc16::seed_mode_ccitt:
        return ccitt_seed;

    case srecord::crc16::seed_mode_xmodem:
        return xmodem_seed;

    case srecord::crc16::seed_mode_broken:
        return broken_seed;
    }
    return ccitt_seed;
}


srecord::crc16::crc16(
    seed_mode_t seed_mode,
    bool a_augment,
    unsigned short a_polynomial,
    bit_direction_t a_bitdir
) :
    state(state_from_seed_mode(seed_mode)),
    augment(a_augment),
    polynomial(a_polynomial),
    bitdir(a_bitdir)
{
    calculate_table();
}


srecord::crc16::crc16(const crc16 &rhs) :
    state(rhs.state),
    augment(rhs.augment),
    polynomial(rhs.polynomial),
    bitdir(rhs.bitdir)
{
    for (size_t j = 0; j < 256; ++j)
        table[j] = rhs.table[j];
}


srecord::crc16 &
srecord::crc16::operator=(const crc16 &rhs)
{
    if (this != &rhs)
    {
        state = rhs.state;
        augment = rhs.augment;
        polynomial = rhs.polynomial;
        bitdir = rhs.bitdir;
        for (size_t j = 0; j < 256; ++j)
            table[j] = rhs.table[j];
    }
    return *this;
}


srecord::crc16::~crc16()
{
}


#if 0

//
// This is the simplest possible implementation.  It can be used to
// validate the two following table-driven implementations.
//

inline unsigned short
srecord::crc16::updcrc(unsigned char c, unsigned short state)
    const
{
    if (bitdir == bit_direction_most_to_least)
    {
        for (unsigned i = 0; i < 8; ++i)
        {
            bool xor_flag = !!(state & 0x8000);
            state <<= 1;
            if (c & 0x80)
                state |= 1;
            if (xor_flag)
                state ^= polynomial;
            c <<= 1;
        }
    }
    else
    {
        // note: calculate_table() already reversed the bits in the polynomial
        for (unsigned i = 0; i < 8; ++i)
        {
            bool xor_flag = !!(state & 1);
            state >>= 1;
            if (c & 1)
                state |= 0x8000;
            if (xor_flag)
                state ^= polynomial;
            c >>= 1;
        }
    }
    return state;
}

#endif
#if 1

//
// This version of updcrc doesn't augment automagically, you must
// do it explicitly in the get() method.  It is a more intuitave
// implementation than the "augmentation included" implementation below.
//
// See "A painless guide to CRC error detection algorithms",
// chapter 10, http://www.repairfaq.org/filipg/LINK/F_crc_v33.html#CRCV_002
// for an explanation.
//

inline unsigned short
srecord::crc16::updcrc(unsigned char c, unsigned short state)
    const
{
    if (bitdir == bit_direction_least_to_most)
    {
        return (((state >> 8) & 0xFF) | (c << 8)) ^ table[state & 0xFF];
    }
    else
    {
        return ((state << 8) | c) ^ table[state >> 8];
    }
}

#endif
#if 0

//
// This version of updcrc means that the 16-zero-bit augmentation has
// already happened.  There is no need to explicitly do it in the get()
// method.  This is deep voodoo even for folks who actually understand
// XOR arithmetic.
//
// See "A painless guide to CRC error detection algorithms",
// chapter 11, http://www.repairfaq.org/filipg/LINK/F_crc_v33.html#CRCV_003
// for an explanation.
//

inline unsigned short
srecord::crc16::updcrc(unsigned char c, unsigned short state)
    const
{
    if (bitdir == bit_direction_least_to_most)
        return (state >> 8) ^ table[(state ^ c) & 0xFF];
    else
        return (state << 8) ^ table[(state >> 8) ^ c];
}

#endif


void
srecord::crc16::next(unsigned char ch)
{
    state = updcrc(ch, state);
}


void
srecord::crc16::nextbuf(const void *data, size_t nbytes)
{
    unsigned char *dp = (unsigned char *)data;
    while (nbytes > 0)
    {
        state = updcrc(*dp++, state);
        --nbytes;
    }
}


unsigned short
srecord::crc16::get()
    const
{
    if (augment)
    {
        return updcrc(0, updcrc(0, state));
    }
    return state;
}


#include <cstdio>


void
srecord::crc16::print_table()
    const
{
    printf("/*\n");
    printf
    (
        " * Bit order: %s\n",
        (
            bitdir == bit_direction_most_to_least
        ?
            "most to least"
        :
            "least to most"
        )
    );
    printf(" * Polynomial: 0x");
    if (bitdir == bit_direction_most_to_least)
        printf("%04X", polynomial);
    else
        printf("%04X", bitrev16(polynomial));
    printf("\n */\n");
    printf("const unsigned short table[256] =\n{\n");
    for (size_t j = 0; j < 256; ++j)
    {
        if ((j & 7) == 0)
            printf("    /* %02X */", int(j));
        printf(" 0x%04X,", table[j]);
        if ((j & 7) == 7)
            printf("\n");
    }
    printf("};\n");
}
