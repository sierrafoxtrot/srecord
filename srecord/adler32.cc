//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009-2011 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <srecord/adler32.h>


srecord::adler32::~adler32()
= default;


srecord::adler32::adler32() 
    
= default;


srecord::adler32::adler32(const adler32 &rhs) 
    
= default;


auto
srecord::adler32::operator=(const adler32 &rhs) -> srecord::adler32 &
{
    if (this != &rhs)
    {
        sum_a = rhs.sum_a;
        sum_b = rhs.sum_b;
    }
    return *this;
}


auto
srecord::adler32::get()
    const -> unsigned long
{
    return ((((unsigned long)sum_b) << 16) | sum_a);
}


void
srecord::adler32::next(unsigned char c)
{
    // This is not portable to int=16-bit machines
    sum_a = (sum_a + c) % 65521;
    sum_b = (sum_b + sum_a) % 65521;
}


void
srecord::adler32::nextbuf(const void *data, size_t nbytes)
{
    const auto *dp = (const unsigned char *)data;
    while (nbytes > 0)
    {
        next(*dp);
        ++dp;
        --nbytes;
    }
}
