//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2002, 2003, 2006-2008, 2010 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 3 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program. If not, see
//      <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_MEMORY_WALKER_WRITER_H
#define SRECORD_MEMORY_WALKER_WRITER_H

#include <srecord/memory/walker.h>
#include <srecord/output.h>

/**
  * The srec_memory_walker_writer class is used to write data to and
  * output stream when a memory image is walked.
  */
class srec_memory_walker_writer:
    public srec_memory_walker
{
public:
    /**
      * the destructor.
      */
    virtual ~srec_memory_walker_writer();

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param deeper
      *     where to write the output
      */
    srec_memory_walker_writer(const srec_output::pointer &deeper);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     where to write the output
      */
    static pointer create(const srec_output::pointer &deeper);

protected:
    // See base class for documentation.
    virtual void observe(unsigned long, const void *, int);

    // See base class for documentation.
    void notify_upper_bound(long unsigned);

    // See base class for documentation.
    void observe_header(const srec_record *);

    // See base class for documentation.
    void observe_start_address(const srec_record *);

private:
    /**
      * The op instance variabel is used to remember where to send
      * the output.
      */
    srec_output::pointer op;

    /**
      * The default constructor.  Do not use.
      */
    srec_memory_walker_writer();

    /**
      * The copy constructor.  Do not use.
      */
    srec_memory_walker_writer(const srec_memory_walker_writer &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_memory_walker_writer &operator=(const srec_memory_walker_writer &);
};

#endif // SRECORD_MEMORY_WALKER_WRITER_H
