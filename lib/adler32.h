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

#ifndef LIB_ADLER32_H
#define LIB_ADLER32_H

#include <stddef.h>

/**
  * The adler32 class is used to represent the running value of a 32-bit
  * cyclic redundancy check of series of bytes.
  */
class adler32
{
public:
    /**
      * The destructor.
      */
    virtual ~adler32();

    /**
      * The default constructor.
      */
    adler32();

    /**
      * The copy constructor.
      */
    adler32(const adler32 &);

    /**
      * The assignment operator.
      */
    adler32 &operator=(const adler32 &);

    /**
      * The get method is used to obtain the running value of the cyclic
      * redundancy check.
      */
    unsigned long get() const;

    /**
      * The next method is used to advance the state by one byte.
      */
    void next(unsigned char);

    /**
      * The nextbuf method is used to advance the state by a series of bytes.
      */
    void nextbuf(const void *, size_t);

private:
    unsigned short sum_a;
    unsigned short sum_b;
};

// vim:ts=8:sw=4:et
#endif // LIB_ADLER32_H
