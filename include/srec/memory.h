//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998-2003 Peter Miller;
//	All rights reserved.
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
// MANIFEST: interface definition for lib/srec/memory.cc
//

#ifndef INCLUDE_SREC_MEMORY_H
#define INCLUDE_SREC_MEMORY_H

#pragma interface "srec_memory"

#include <srec/memory/chunk.h>

class srec_record; // forward

/**
  * The srec_memory class is used to simulate memory contents.
  * This allows staging of reord adta, and also validation of contents.
  */
class srec_memory
{
public:
    /**
      * The default constructor.
      */
    srec_memory();

    /**
      * The copy constructor.
      */
    srec_memory(const srec_memory &);

    /**
      * The assignment operator.
      */
    srec_memory &operator=(const srec_memory &);

    /**
      * The destructor.
      */
    ~srec_memory();

    /**
      * The set method is used to set the byte at the given `address'
      * to the given `value'.
      *
      * Uses the find() method to locate the chunk, and then calls
      * the srec_memory_chunk::set method, to set the byte within
      * the chunk.
      */
    void set(unsigned long address, int value);

    /**
      * The get method is used to fetch the value of the byte at
      * the given 'address'.
      *
      * If you fo a get on an address which has not been set() yet,
      * the results are undefined.
      *
      * Uses the find() method to locate the chunk, and then calls
      * the srec_memory_chunk::get method, to get the byte within
      * the chunk.
      */
    int get(unsigned long address) const;

    /**
      * The set_p method is used to determine whether the byte at
      * the given address has been set() yet.  Returns true if
      * already set, false if never been set.
      *
      * Uses the find() method to locate the chunk, and then calls
      * the srec_memory_chunk::set_p method, to get the status of
      * the byte within the chunk.
      */
    bool set_p(unsigned long address) const;

    /**
      * The walk method is used to apply a srec_memory_walker derived
      * class to every byte of memory.
      */
    void walk(class srec_memory_walker *) const;

    /**
      * The reader method is used to read the given `input' source
      * into memory.  This method may be called multiple times,
      * for multiple input files.
      *
      * If the input format supports headers, and the file has a
      * header, the first header will be remembered, if set_header()
      * was not called previously.
      *
      * If the value at any address is set more than once, a fatal
      * error will be issued, informing the user of the address
      * and the contradictory values.  If the allow_overwriting()
      * method has been called previously, this will be a non-fatal
      * warning, instead.  If `barf' is false, no checking of any
      * kind is performed.
      */
    void reader(class srec_input *input, bool barf = false);

    /**
      * The equal method may be used to determine if two srec_memory
      * instances are equal.
      */
    static bool equal(const srec_memory &, const srec_memory &);

    /**
      * The compare method may be used to determine if two srec_memory
      * instances are equal.  Verbose messages concerning the exact
      * differences are printed if they are not equal.
      */
    static bool compare(const srec_memory &, const srec_memory &);

    /**
      * The find_next_data method may be used to locate data at or
      * following the `address' given.  Returns false if there is
      * no data meeting this criteria.
      *
      * If there is data, the `address' will be set to the start
      * address of the data block.	At most `nbytes' of data will
      * be transfetrred into the `data' array.  Then `nbytes' wil
      * be set to the number of bytes transferred.	Returns true.
      *
      * Calls the find_next_chunk() method.
      */
    bool find_next_data(unsigned long &address, void *data,
	size_t &nbytes) const;

    /**
      * The allow_overwriting controls the error message behaviour of
      * the reader() method.  Once called, redundant and contradictory
      * settings generate warnings, rather than fatal errors.
      */
    static void allow_overwriting();

    /**
      * The get_header method is used to determine the value of the
      * header record set by either the reader() or set_header()
      * methods.  If neither has set a header, NULL is returned.
      */
    srec_record *get_header() const;

    /**
      * The set_header method may be used to set the header command.
      * The `value' is a NUL terminated C string.
      */
    void set_header(const char *value);

    /**
      * The get_start_address method is used to determine the value
      * of the start address record set by either the reader()
      * or set_start_address() methods.  If neither has set a
      * start_address, NULL is returned.
      */
    srec_record *get_start_address() const;

    /**
      * The set_start_address method may be used to set the start
      * address record.
      */
    void set_start_address(unsigned long value);

private:
    /**
      * The overwrite static variable is used to remember whether
      * or not we generate warnings when redundant or contradictory
      * settings are deteccted by the reader() method.
      */
    static bool overwrite;

    /**
      * The nchunks instance variable is used to member how large
      * our pool of memory chunks is.  It is always <= nchunks_max;
      */
    mutable int nchunks;

    /**
      * The max_chunks instance variable is used to rememberf the
      * size of the chunk array, which holds the pool of memory chunks.
      */
    mutable int nchunks_max;

    /**
      * The chunk instance variable is used to hold the pointer to
      * the base of the chunk array.  These chuinks remember the
      * settings of the various bytes.  By using a sparse array,
      * we can cope with arbitrary memory usages.
      */
    mutable srec_memory_chunk **chunk;

    /**
      * The find method is used to find the chunk which contains
      * the given `address'.  The chunk will be created if it
      * doesn't exist.
      *
      * Called by the get(), set() and set_p() methods.
      */
    srec_memory_chunk *find(unsigned long address) const;

    /**
      * The cache instance variable is used to accellerate the find()
      * method, based on the fact that most memory accesses are
      * sequential, in the same chunk.
      */
    mutable srec_memory_chunk *cache;

    /**
      * The find_next_chunk method is used to visit each and every
      * byte, in cases where the walk() method is not appropriate.
      * Called by the find_next_data() method.
      */
    srec_memory_chunk *find_next_chunk(unsigned long) const;

    /**
      * The find_next_chunk_index instance variable is used by
      * the find_next_chunk() method to keep track of where it is
      * positioned across the chunk array.
      */
    mutable int find_next_chunk_index;

    /**
      * The header instance variable is used to track the file header.
      * It is set by the reader() and set_header() methods.  It is
      * read by the get_header() method.
      */
    srec_record *header;

    /**
      * The start_address instance variable is used to track the start
      * address.  It is set by the reader() and set_start_address()
      * methods.  It is read by the get_start_address() method.
      */
    srec_record *start_address;

    /**
      * The clear method is used to discard all data, as if when
      * the instance was first constructed. Alsu used by the destructor.
      */
    void clear();

    /**
      * The copy method is used to copy the chunks from the `src' into
      * this object.  Only to be used the the assignment operator.
      */
    void copy(const srec_memory &src);
};

/**
  * The equality operator.  Used to test if two srec_memory objects
  * are equal.  Calls the srec_memory::equal() method.
  */
bool operator == (const srec_memory &, const srec_memory &);

/**
  * The inequality operator.  Used to test if two srec_memory objects
  * are not equal.  Calls the srec_memory::equal() method.
  */
bool operator != (const srec_memory &, const srec_memory &);

#endif // INCLUDE_SREC_MEMORY_H
