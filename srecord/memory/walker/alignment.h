//
// srecord - Manipulate EPROM load files
// Copyright (C) 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_MEMORY_WALKER_ALIGNMENT_H
#define SRECORD_MEMORY_WALKER_ALIGNMENT_H

#include <srecord/memory/walker.h>

namespace srecord
{

/**
  * The srecord::memory_walker_alignment class is used to represent the
  * parse state of a memory walker which determines whether or not the
  * data are well aligned.
  */
class memory_walker_alignment:
    public memory_walker
{
public:
    typedef std::shared_ptr<memory_walker_alignment> pointer;

    /**
      * The destructor.
      */
    ~memory_walker_alignment() override = default;

    /**
      * The create class method is used to create new dynamically
      * allocated instances of class.
      *
      * @param multiple
      *     The multiple of bytes we expect for alignment.
      */
    static pointer create(unsigned multiple);

    /**
      * The is_continuous method is used to get the results of the
      * calculation.
      *
      * @returns
      *     true if the data has no holes, false if there are holes
      */
    bool is_well_aligned() const;

protected:
    // See base class for documentation.
    void observe(uint32_t, const void *, int) override;

    // See base class for documentation.
    void observe_end() override;

private:
    /**
      * The constructor.
      * It is private on purpose, use the #create class method instead.
      *
      * @param multiple
      *     The multiple of bytes we expect for alignment.
      */
    memory_walker_alignment(unsigned multiple);

    unsigned multiple{2};
    uint32_t current_address{0};
    bool data_seen{false};
    bool well_aligned{true};

public:
    /**
      * The default constructor.
      * Do not use.
      */
    memory_walker_alignment() = delete;

    /**
      * The copy constructor.  No not use.
      */
    memory_walker_alignment(const memory_walker_alignment &) = delete;

    /**
      * The assignment operator.  No not use.
      */
    memory_walker_alignment &operator=(
        const memory_walker_alignment &) = delete;
};

};

#endif // SRECORD_MEMORY_WALKER_ALIGNMENT_H
