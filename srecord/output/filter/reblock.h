//
// srecord - Manipulate EPROM load files
// Copyright (C) 2010, 2011 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILTER_REBLOCK_H
#define SRECORD_OUTPUT_FILTER_REBLOCK_H

#include <srecord/output/filter.h>

namespace srecord {

/**
  * The output_filter_reblock class is used to represent a filter that
  * repacks output records so that they exactly align with the output
  * format's preferred block size.
  *
  * This can also be used to remove artifacts of whatever the SRecord
  * internal memory chunk size happens to be.
  */
class output_filter_reblock:
    public output_filter
{
public:
    typedef std::shared_ptr<output_filter_reblock> pointer;

    /**
      * The destructor.
      */
    ~output_filter_reblock() override;

    /**
      * The create class method is used to create new dynamically
      * allocated instanced of this class.
      *
      * @param deeper
      *     Where to send our filtered output.
      * @param align
      *     Whether or not to align data records on block boundaries
      *     (that is, to use non-optimal short writes after holes to force
      *     alignment).
      */
    static pointer create(const output::pointer &deeper, bool align = false);

protected:
    // See base class for documentation.
    void write(const record &r) override;

    // See base class for documentation.
    void line_length_set(int) override;

    // See base class for documentation.
    void address_length_set(int) override;

    // See base class for documentation.
    int preferred_block_size_get() const override;

    // See base class for documentation.
    bool preferred_block_size_set(int nbytes) override;

private:
    /**
      * The constructor.
      * It is private on purpose, use the #create class method instead.
      *
      * @param deeper
      *     Where to send our filtered output.
      * @param align
      *     Whether or not to align data records on block boundaries
      *     (that is, to use non-optimal short writes after holes to force
      *     alignment).
      */
    output_filter_reblock(const output::pointer &deeper, bool align);

    /**
      * The align instance variable is used to remember whether or
      * not to use short writes after holds to force records to be
      * on block_size boundaries, rather than as tightly packed as
      * possible.
      */
    bool align;

    /**
      * The block_size instance variable is used to remember the
      * block size (in bytes) we are packing to.  It is cache of the
      * deeper->preferred_block_size_get() value, the cache is updated
      * at strategic times, by calling the #track_block_size method.
      */
    unsigned block_size{0};

    /**
      * The buffer_address instance variable is used to remember the
      * byte address of the first byte of the output buffer.  Not
      * meaningful if #buffer_pos is zero.
      */
    uint32_t buffer_address{0};

    /**
      * The buffer instance variable is used to remember the base
      * address of a buffer allocated from dynamic memory.  The
      * allocated size may be found in the #buffer_max instance
      * variable.
      *
      * assert(!buffer == !buffer_max);
      */
    uint8_t *buffer{0};

    /**
      * The buffer_pos instance variable is used to remember how many
      * bytes of the #buffer have been used to date.
      *
      * assert(buffer_pos < buffer_max);
      */
    size_t buffer_pos{0};

    /**
      * The buffer_max instance variable is used to remember how many
      * data bytes were allocated when the #buffer was created.
      */
    size_t buffer_max{0};

    /**
      * The track_block_size method is used to re-cache the
      * deeper->preferred_bock_size_get whenever that value could have
      * changed.
      */
    void track_block_size();

    /**
      * The flush_buffer method is used to write the accumulated
      * contents of the #buffer to the deeper output.
      *
      * @param partial
      *     Whether or not to write a partial record at the end.  True
      *     for partial record, false for no short record (which could
      *     leave some residual bytes in the #buffer).
      */
    void flush_buffer(bool partial);

public:
    /**
      * The default constructor.
      */
    output_filter_reblock() = delete;

    /**
      * The copy constructor.
      */
    output_filter_reblock(const output_filter_reblock &) = delete;

    /**
      * The assignment operator.
      */
    output_filter_reblock &operator=(const output_filter_reblock &) = delete;
};

};

#endif // SRECORD_OUTPUT_FILTER_REBLOCK_H
