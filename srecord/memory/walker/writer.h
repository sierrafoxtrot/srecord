//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2002, 2003, 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_MEMORY_WALKER_WRITER_H
#define SRECORD_MEMORY_WALKER_WRITER_H

#include <srecord/memory/walker.h>
#include <srecord/output.h>

namespace srecord {

/**
  * The srecord::memory_walker_writer class is used to write data to an
  * output stream when a memory image is walked.
  */
class memory_walker_writer:
    public memory_walker
{
public:
    /**
      * the destructor.
      */
    ~memory_walker_writer() override;

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param deeper
      *     where to write the output
      */
    memory_walker_writer(output::pointer arg);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     where to write the output
      */
    static auto create(const output::pointer &arg) -> pointer;

protected:
    // See base class for documentation.
    void observe(unsigned long, const void *, int) override;

    // See base class for documentation.
    void notify_upper_bound(long unsigned) override;

    // See base class for documentation.
    void observe_header(const record *) override;

    // See base class for documentation.
    void observe_start_address(const record *) override;

private:
    /**
      * The op instance variable is used to remember where to send
      * the output.
      */
    output::pointer op;

    /**
      * The default constructor.  Do not use.
      */
    memory_walker_writer() = delete;

    /**
      * The copy constructor.  Do not use.
      */
    memory_walker_writer(const memory_walker_writer &) = delete;

    /**
      * The assignment operator.  Do not use.
      */
    auto operator=(const memory_walker_writer &) -> memory_walker_writer & = delete;
};

}

#endif // SRECORD_MEMORY_WALKER_WRITER_H
