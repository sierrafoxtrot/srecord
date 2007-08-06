//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2002, 2006, 2007 Peter Miller
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

#include <lib/crc16.h>

//
// The CRC polynomial.  This is used by CCITT and XMODEM, but not X25.
//
#define POLYNOMIAL 0x1021

// #define EXPLICIT_AUGMENTATION 1

static unsigned short table[256];

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
#ifdef EXPLICIT_AUGMENTATION
static unsigned short const ccitt_seed = 0xFFFF;
#else
static unsigned short const ccitt_seed = 0x1D0F;
#endif

static unsigned short const xmodem_seed = 0;


static void
calculate_table()
{
    for (int b = 0; b < 256; ++b)
    {
        unsigned short v = b << 8;
        for (int i = 8; --i >= 0; )
            v = (v & 0x8000) ? ((v << 1) ^ POLYNOMIAL) : (v << 1);
        table[b] = v;
    }
}


crc16::crc16(bool ccitt) :
    state(0)
{
    if (ccitt)
        state = ccitt_seed;
    else
        state = xmodem_seed;

    if (!table[1])
        calculate_table();
}


crc16::crc16(const crc16 &arg) :
    state(arg.state)
{
}


crc16 &
crc16::operator=(const crc16 &arg)
{
    if (this != &arg)
    {
        state = arg.state;
    }
    return *this;
}


crc16::~crc16()
{
}


#if 0

//
// This is the simplest possible implementation.  It can be used to
// validate the two following table-driven implementations.
//
static unsigned short
updcrc(unsigned char c, unsigned short state)
{
    for (unsigned i = 0; i < 8; ++i)
    {
        bool xor_flag = !!(state & 0x8000);
        state <<= 1;
        if (c & 0x80)
            state |= 1;
        if (xor_flag)
            state ^= POLYNOMIAL;
        c <<= 1;
    }
    return state;
}

#endif


#ifdef EXPLICIT_AUGMENTATION

//
// This version of updcrc doesn't augment automagically, you must
// do it explicitly in the get() method.  It is a more intuitave
// implementation and the "augmentation included" implementation below.
//
// See "A painless guide to CRC error detection algorithms",
// chapter 10, http://www.repairfaq.org/filipg/LINK/F_crc_v33.html#CRCV_002
// for an explanation.
//
static inline unsigned short
updcrc(unsigned char c, unsigned short state)
{
    return ((state << 8) | c) ^ table[state >> 8]
}

#else

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

static inline unsigned short
updcrc(unsigned char c, unsigned short state)
{
    return (state << 8) ^ table[(state >> 8) ^ c];
}

#endif


void
crc16::next(unsigned char x)
{
    state = updcrc(x, state);
}


void
crc16::nextbuf(const void *data, size_t nbytes)
{
    unsigned char *dp = (unsigned char *)data;
    while (nbytes > 0)
    {
        state = updcrc(*dp, state);
        ++dp;
        --nbytes;
    }
}


unsigned short
crc16::get()
    const
{
#ifdef EXPLICIT_AUGMENTATION
    return updcrc(0, updcrc(0, state));
#else
    return state;
#endif
}
