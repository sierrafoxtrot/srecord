//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998, 1999, 2001, 2002, 2004 Peter Miller;
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
// MANIFEST: interface definition for lib/srec/input/filter/fill.cc
//

#ifndef INCLUDE_SREC_INPUT_FILTER_RANDOM_FILL_H
#define INCLUDE_SREC_INPUT_FILTER_RANDOM_FILL_H

#pragma interface "srec_input_filter_random_fill"

#include <interval.h>
#include <srec/input/filter.h>
#include <srec/record.h>

/**
  * The srec_input_filter_random_fill class is used to represent a filter
  * which replaces in set data locations with random data.
  */
class srec_input_filter_random_fill:
    public srec_input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_filter_random_fill();

    /**
      * The constructor.
      */
    srec_input_filter_random_fill(srec_input *, const interval &);

    // See base class for documentation.
    virtual int read(srec_record &);

private:
    /**
      * The range instance variable is used to remember the range of
      * addresses to be filled.  As fill blocks are produced the range
      * is reduced.
      */
    interval range;

    /**
      * The generate method is used to genetate fill records.
      */
    int generate(srec_record &);

    /**
      * The default constructor.  Do not use.
      */
    srec_input_filter_random_fill();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_filter_random_fill(const srec_input_filter_random_fill &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_filter_random_fill &operator=(
	const srec_input_filter_random_fill &);
};

#endif // INCLUDE_SREC_INPUT_FILTER_RANDOM_FILL_H
