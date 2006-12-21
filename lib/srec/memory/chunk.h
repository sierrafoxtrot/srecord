//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998-2000, 2002, 2003, 2006 Peter Miller
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: interface definition for lib/srec/memory/chunk.cc
//

#ifndef INCLUDE_SREC_MEMORY_CHUNK_H
#define INCLUDE_SREC_MEMORY_CHUNK_H


#include <stddef.h>

/**
  * The srec_memory_chunk class is used to represent portion of memory.
  * Not all bytes are actually set, so there is a bit map of which bytes
  * actually contain data.
  */
class srec_memory_chunk
{
public:
    enum {
    /**
      * The size value is the size, in bytes, of each memory chunk.
      */
    size = 7 * 256 };

    /**
      * The constructor.
      */
    srec_memory_chunk(unsigned long address);

    /**
      * The copy constructor.
      */
    srec_memory_chunk(const srec_memory_chunk &);

    /**
      * The assignment operator.
      */
    srec_memory_chunk &operator=(const srec_memory_chunk &);

    /**
      * The destructor.
      */
    ~srec_memory_chunk();

    /**
      * The set method is used to set the byte at the given offset within
      * the chunk.
      */
    void set(unsigned long offset, int value);

    /**
      * The get method is used to get the value at the given offset
      * within the chunk.
      */
    int get(unsigned long offset);

    /**
      * The get_p method is used to determine whether the byte at the
      * given offset within the chunk contains valid data.
      */
    bool set_p(unsigned long) const;

    /**
      * The walk method is used to iterate across all of the bytes which
      * are set within the chunk, calling the walker's observe method.
      */
    void walk(class srec_memory_walker *) const;

    /**
      * The get_address method is used to get the address of the memory
      * chunk.	This is NOT the address of the first byte, it is the
      * chunk number.  To calculate the byte address, multiply by size.
      */
    unsigned long get_address() const { return address; }

    /**
      * The equal class method is used to determine wherther two memory
      * chunks are equal.  The must have the same address, the same bit
      * mask, and the same byte values on the valid bytes.
      */
    static bool equal(const srec_memory_chunk &, const srec_memory_chunk &);

    /**
      * The find_next_data method is used when iteratinbg across all of
      * the bytes set within the chunk.
      */
    bool find_next_data(unsigned long &, void *, size_t &) const;

    /**
      * The get_upper_bound method is used to determine the upper bound
      * (offset of last byte with valid data, plus one) of the chunk.
      * It returns a memory byte address, NOT the chunk offset.
      */
    unsigned long get_upper_bound() const;

private:
    /**
      * The address of the memory chunk.  This is NOT the address of
      * the first byte, it is the chunk number.  To calculate the byte
      * address, multiply by size.
      */
    unsigned long address;

    /**
      * The data array is used to remember the valus of valid data bytes.
      */
    unsigned char data[size];

    /**
      * The mask array is used to remember which values in the data
      * array contain valid values.
      */
    unsigned char mask[(size + 7) / 8];

    /**
      * The default constructor.  Do not use.
      */
    srec_memory_chunk();
};

bool operator == (const srec_memory_chunk &, const srec_memory_chunk &);
bool operator != (const srec_memory_chunk &, const srec_memory_chunk &);

#endif // INCLUDE_SREC_MEMORY_CHUNK_H
