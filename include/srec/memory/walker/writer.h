//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998, 1999, 2002, 2003 Peter Miller;
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
// MANIFEST: interface definition for lib/srec/memory/walker/writer.cc
//

#ifndef INCLUDE_SREC_MEMORY_WALKER_WRITER_H
#define INCLUDE_SREC_MEMORY_WALKER_WRITER_H

#pragma interface "srec_memory_walker_writer"

#include <srec/memory/walker.h>

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

    /**
      * The constructor.
      */
    srec_memory_walker_writer(class srec_output *);

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
    class srec_output *op;

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

#endif // INCLUDE_SREC_MEMORY_WALKER_WRITER_H
