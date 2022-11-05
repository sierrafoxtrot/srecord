//
// srecord - manipulate eprom load files
// Copyright (C) 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_MEMORY_WALKER_CONTINUITY_H
#define SRECORD_MEMORY_WALKER_CONTINUITY_H

#include <srecord/memory/walker.h>

namespace srecord
{

/**
  * The srecord::memory_walker_crc16 class is used to represent the parse
  * state of a memory walker which determines whether or not the data
  * are continuous.
  */
class memory_walker_continuity:
    public memory_walker
{
public:
    using pointer = std::shared_ptr<memory_walker_continuity>;

    /**
      * The destructor.
      */
    ~memory_walker_continuity() override;

private:
    /**
      * The default constructor.  It is private on purpose, use the
      * #create class method instead.
      */
    memory_walker_continuity();

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of class.
      */
    static auto create() -> pointer;

    /**
      * The is_continuous method is used to get the results of the
      * calculation.
      *
      * @returns
      *     true if the data has no holes, false if there are holes
      */
    auto is_continuous() const -> bool;

protected:
    // See base class for documentation.
    void observe(unsigned long, const void *, int) override;

private:
    unsigned long current_address{0};
    bool data_seen{false};
    int nholes{0};

    /**
      * The copy constructor.  No not use.
      */
    memory_walker_continuity(const memory_walker_continuity &) = delete;

    /**
      * The assignment operator.  No not use.
      */
    auto operator=(const memory_walker_continuity &) -> memory_walker_continuity & = delete;
};

};

#endif // SRECORD_MEMORY_WALKER_CONTINUITY_H
