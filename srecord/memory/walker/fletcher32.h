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

#ifndef SRECORD_MEMORY_WALKER_FLETCHER32_H
#define SRECORD_MEMORY_WALKER_FLETCHER32_H

#include <srecord/fletcher32.h>
#include <srecord/memory/walker.h>

namespace srecord
{

/**
  * The srecord::memory_walker_fletcher32 class is used to represent the parse
  * state of a memory walker which calculates a running Fletcher-32 checksum.
  */
class memory_walker_fletcher32:
    public memory_walker
{
public:
    typedef std::shared_ptr<memory_walker_fletcher32> pointer;

    /**
      * The destructor.
      */
    ~memory_walker_fletcher32() override  = default;

private:
    /**
      * The default constructor.  It is private on purpose, use the
      * #create method instead.
      */
    memory_walker_fletcher32() = default;

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create();

    /**
      * The get method is used to get the Fletcher-32 checksum once all memory
      * chunks have been processed by calls to our observe method.
      */
    unsigned get() const;

protected:
    // See base class for documentation.
    void observe(uint32_t, const void *, int) override;

private:
    /**
      * The checksum instance variable is used to remember the running
      * state of the Fletcher-32 checksum calculation.
      */
    fletcher32 checksum;

public:
    /**
      * The copy constructor.
      */
    memory_walker_fletcher32(const memory_walker_fletcher32 &) = delete;

    /**
      * The assignment operator.
      */
    memory_walker_fletcher32 &operator=(
        const memory_walker_fletcher32 &) = delete;
};

};

#endif // SRECORD_MEMORY_WALKER_FLETCHER32_H
