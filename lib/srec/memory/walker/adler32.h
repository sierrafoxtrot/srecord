//
// srecord - manipulate eprom load files
// Copyright (C) 2009 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//

#ifndef LIB_SREC_MEMORY_WALKER_ADLER32_H
#define LIB_SREC_MEMORY_WALKER_ADLER32_H

#include <lib/adler32.h>
#include <lib/srec/memory/walker.h>

/**
  * The srec_memory_walker_adler32 class is used to represent the parse
  * state of a memory walker which calculates a running ADLER32 checksum.
  */
class srec_memory_walker_adler32:
    public srec_memory_walker
{
public:
    typedef boost::shared_ptr<srec_memory_walker_adler32> pointer;

    /**
      * The destructor.
      */
    virtual ~srec_memory_walker_adler32();

private:
    /**
      * The default constructor.  It is private on putpose, use the
      * #create method instead.
      */
    srec_memory_walker_adler32();

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create();

    /**
      * The get method is used to get the ADLER32 checksum once all memory
      * chunks have been processed by calls to our observe method.
      */
    unsigned get() const;

protected:
    // See base class for documentation.
    void observe(unsigned long, const void *, int);

private:
    /**
      * The checksum instance variable is used to remember the running
      * state of the ADLER32 checksum calculation.
      */
    adler32 checksum;

    /**
      * The copy constructor.  Do not use.
      */
    srec_memory_walker_adler32(const srec_memory_walker_adler32 &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_memory_walker_adler32 &operator=(const srec_memory_walker_adler32 &);
};

#endif // LIB_SREC_MEMORY_WALKER_ADLER32_H
