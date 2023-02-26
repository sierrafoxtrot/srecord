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

uint32_t srecord::adler32::get() const
{
    return ((((uint32_t)sum_b) << 16) | sum_a);
}


void
srecord::adler32::next(uint8_t c)
{
    // This is not portable to int=16-bit machines
    sum_a = (sum_a + c) % 65521;
    sum_b = (sum_b + sum_a) % 65521;
}


void
srecord::adler32::nextbuf(const void *data, size_t nbytes)
{
    const auto *dp = (const uint8_t *)(data);
    while (nbytes > 0)
    {
        next(*dp);
        ++dp;
        --nbytes;
    }
}
