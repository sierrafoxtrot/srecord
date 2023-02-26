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

#ifndef SRECORD_ADLER32_H
#define SRECORD_ADLER32_H

#include <cstddef>
#include <cstdint>

namespace srecord
{

/**
  * The adler32 class is used to represent the running value of a 32-bit
  * Adler checksum of series of bytes.
  *
  * See wikipedia for a description
  * http://en.wikipedia.org/wiki/Adler-32
  */
class adler32
{
public:
    /**
      * The get method is used to obtain the running value of the
      * checksum.
      */
    uint32_t get() const;

    /**
      * The next method is used to advance the state by one byte.
      */
    void next(uint8_t);

    /**
      * The nextbuf method is used to advance the state by a series of bytes.
      */
    void nextbuf(const void *, size_t);

private:
    /**
      * The sum_a instance variable is used to remember the sum of bytes
      * scanned.
      */
    uint16_t sum_a{1};

    /**
      * The sum_b instance variable is used to remember the sum of the
      * sum of bytes scanned.
      */
    uint16_t sum_b{0};
};

};

#endif // SRECORD_ADLER32_H
