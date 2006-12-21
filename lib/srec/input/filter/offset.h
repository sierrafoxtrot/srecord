//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998, 1999, 2001, 2002, 2005, 2006 Peter Miller
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
// MANIFEST: interface definition for lib/srec/input/filter/offset.cc
//

#ifndef INCLUDE_SREC_INPUT_FILTER_OFFSET_H
#define INCLUDE_SREC_INPUT_FILTER_OFFSET_H


#include <lib/srec/input/filter.h>

/**
  * The srec_input_filter_offset class is used to represent a filetr
  * where the addresses of the the deeper input source are offset by a
  * number of bytes.
  */
class srec_input_filter_offset:
    public srec_input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_filter_offset();

    /**
      * The constructor.
      *
      * @param deeper
      *     The deeper input stream.
      * @param nbytes
      *     The number of bytes to offset the addresses by.
      *     The value may be negative.
      */
    srec_input_filter_offset(srec_input *deeper, long nbytes);
    virtual int read(srec_record &);

private:
    /**
      * The numberof bytes to affset the data by.
      */
    long nbytes;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_filter_offset();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_filter_offset(const srec_input_filter_offset &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_filter_offset &operator=(const srec_input_filter_offset &);
};

#endif // INCLUDE_SREC_INPUT_FILTER_OFFSET_H
