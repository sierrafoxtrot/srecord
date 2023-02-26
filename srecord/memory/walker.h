//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2002, 2003, 2006-2008, 2010, 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_MEMORY_WALKER_H
#define SRECORD_MEMORY_WALKER_H

#include <memory>

namespace srecord {

class record; // forward

/**
  * The srecord::memory_walker class is used to represent an abstract handler
  * for the action to perform when walking a memory image.
  */
class memory_walker
{
public:
    typedef std::shared_ptr<memory_walker> pointer;

    /**
      * The destructor.
      */
    virtual ~memory_walker() = default;

    /**
      * The observe method is used by the memory walker to provide data.
      * Derived classes are required to implement this method, and do
      * something with the data.
      *
      * @param address
      *     The base address of this chunk of memory.
      * @param data
      *     The base address of this chunk of memory in memory.
      * @param data_size
      *     The size, in bytes, of this chunk of memory.
      */
    virtual void observe(uint32_t address, const void *data,
        int data_size) = 0;

    /**
      * The observe_end method is called once all of the data blocks
      * have been passed to the observer via the observe() method.
      */
    virtual void observe_end();

    /**
      * The notify_upper_bound method is used to notify the walker of
      * the upper bound (highest address plus one) of the observe calls
      * to come.  Shall be called before the any observe calls are made.
      * By default, nothing happens.
      *
      * @param address
      *     The address of the byte immediately beyond the used memory.
      */
    virtual void notify_upper_bound(uint32_t address);

    /**
      * The observe_header method is used to inform the walker of the
      * header record.  The default does nothing.
      *
      * @param rec
      *     The record to be processed.
      */
    virtual void observe_header(const record *rec = 0);

    /**
      * The observe_start_address method is used to inform the walker
      * of the execution start address record.  The default does nothing.
      *
      * @param rec
      *     The record to be processed.
      */
    virtual void observe_start_address(const record *rec = 0);

protected:
    /**
      * The default constructor.  May only be called by derived classes.
      */
    memory_walker() = default;

public:
    /**
      * The copy constructor.
      */
    memory_walker(const memory_walker &) = delete;

    /**
      * The assignment operator.
      */
    memory_walker &operator=(const memory_walker &) = delete;
};

};

#endif // SRECORD_MEMORY_WALKER_H
