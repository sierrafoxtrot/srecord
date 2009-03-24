//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009 Peter Miller
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

#include <lib/fletcher16.h>


fletcher16::~fletcher16()
{
}


fletcher16::fletcher16() :
    sum1(0xFF),
    sum2(0xFF)
{
    // See notes in nextbuf() method, below.
}


fletcher16::fletcher16(const fletcher16 &rhs) :
    sum1(rhs.sum1),
    sum2(rhs.sum2)
{
}


fletcher16 &
fletcher16::operator=(const fletcher16 &rhs)
{
    if (this != &rhs)
    {
        sum1 = rhs.sum1;
        sum2 = rhs.sum2;
    }
    return *this;
}


void
fletcher16::next(unsigned char ch)
{
    // reduction step to reduce sums to 8 bits
    sum1 += ch;
    sum2 += sum1;
    sum1 = (sum1 & 0xFF) + (sum1 >> 8);
    sum2 = (sum2 & 0xFF) + (sum2 >> 8);
}


void
fletcher16::nextbuf(const void *vdata, size_t nbytes)
{
    //
    // A few tricks, well-known to implementors of the IP checksum, are
    // used here for efficiency:
    //
    // * This reduces to the range 1..255 rather than 0..254.  Modulo
    // 255, the values 255 = 0xFF and 0 are equivalent, but it is easier
    // to detect overflow if the former convention is used.  This also
    // provides the guarantee that the resultant checksum will never
    // be zero, so that value is available for a special flag, such as
    // "checksum not yet computed".
    //
    // * 256 ≡ 1 mod 255, so the end-around carry expression (x &
    // 0xFF) + (x >> 8) reduces x modulo 255.  Only doing it once is not
    // guaranteed to be complete, but it will be less than 0x1fe.  A
    // second repetition guarantees a fully reduced sum in the range of
    // 1..255.
    //
    // * This uses a 16-bit accumulator to perform a number of sums
    // before doing any modular reduction. The magic value 21 is the
    // largest number of sums that can be performed without numeric
    // overflow in 16 bits. Any smaller value is also permissible; 16
    // may be convenient in many cases.
    //
    const unsigned char *data = (const unsigned char *)vdata;
    size_t len = nbytes;
    while (len)
    {
        size_t tlen = len > 21 ? 21 : len;
        len -= tlen;
        for (;;)
        {
            sum1 += *data++;
            sum2 += sum1;
            --tlen;
            if (!tlen)
                break;
        }
        sum1 = (sum1 & 0xFF) + (sum1 >> 8);
        sum2 = (sum2 & 0xFF) + (sum2 >> 8);
    }
    // Second reduction step to reduce sums to 8 bits
    sum1 = (sum1 & 0xFF) + (sum1 >> 8);
    sum2 = (sum2 & 0xFF) + (sum2 >> 8);
}


unsigned short
fletcher16::get()
    const
{
    return ((sum2 << 8) | sum1);
}


// vim:ts=8:sw=4:et
