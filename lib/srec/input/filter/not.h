//
//	srecord - manipulate eprom load files
//	Copyright (C) 2001, 2002, 2005, 2006 Peter Miller;
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
// MANIFEST: interface definition for lib/srec/input/filter/not.cc
//

#ifndef INCLUDE_SREC_INPUT_FILTER_NOT_H
#define INCLUDE_SREC_INPUT_FILTER_NOT_H


#include <lib/srec/input/filter.h>

/**
  * The srec_input_filter_not class is used to represent an input stream
  * which bit-wise NOTs the data.
  */
class srec_input_filter_not:
    public srec_input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_filter_not();

    /**
      * The constructor.
      *
      * @param deeper
      *     The input source to be filtered.
      */
    srec_input_filter_not(srec_input *deeper);

    // See base class for documentation.
    virtual int read(srec_record &);

private:
    /**
      * The default constructor.  Do not use.
      */
    srec_input_filter_not();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_filter_not(const srec_input_filter_not &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_filter_not &operator=(const srec_input_filter_not &);
};

#endif // INCLUDE_SREC_INPUT_FILTER_NOT_H
