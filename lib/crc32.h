//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000-2003, 2006, 2007 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
//      02110-1301 USA.
//
// MANIFEST: interface definition for include/crc32.cc
//

#ifndef INCLUDE_CRC32_H
#define INCLUDE_CRC32_H


#include <stddef.h>

/**
  * The crc32 class is used to represent the running value of a 32-bit
  * cyclic redundancy check of series of bytes.
  */
class crc32
{
public:
    /**
      * The destructor.
      */
    virtual ~crc32();

    /**
      * The default constructor.
      */
    crc32();

    /**
      * The copy constructor.
      */
    crc32(const crc32 &);

    /**
      * The assignment operator.
      */
    crc32 &operator=(const crc32 &);

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
    /**
      * The state instance variable is used to remember the running
      * value of the 32-bit cyclic redundancy check.
      */
    unsigned long state;
};

#endif // INCLUDE_CRC32_H
