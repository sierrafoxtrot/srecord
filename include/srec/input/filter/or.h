//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998, 1999, 2001, 2002, 2005 Peter Miller;
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
// MANIFEST: interface definition for lib/srec/input/filter/or.cc
//

#ifndef INCLUDE_SREC_INPUT_FILTER_OR_H
#define INCLUDE_SREC_INPUT_FILTER_OR_H

#pragma interface "srec_input_filter_or"

#include <srec/input/filter.h>

/**
  * The srec_input_filter_and clas sis used to represent the input state
  * of a filter which bit-wise ORs all incoming data bytes with a fixed
  * value.
  */
class srec_input_filter_or:
    public srec_input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_filter_or();

    /**
      * The constructor.
      *
      * @param deeper
      *     The deeper input to be used as a data source.
      * @param mask
      *     The value to be bit-wise ORed with each incoming data byte.
      */
    srec_input_filter_or(srec_input *, int);

    // See base class for documentation.
    virtual int read(srec_record &);

private:
    /**
      * The value instance variable is used to remember the value to be
      * bit-wise ORed with each incoming data byte.
      */
    int value;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_filter_or();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_filter_or(const srec_input_filter_or &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_filter_or &operator=(const srec_input_filter_or &);
};

#endif // INCLUDE_SREC_INPUT_FILTER_OR_H
