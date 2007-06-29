//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2001, 2002, 2004, 2006, 2007 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
//      02110-1301 USA.
//
// MANIFEST: interface definition for lib/srec/input/filter/fill.cc
//

#ifndef INCLUDE_SREC_INPUT_FILTER_FILL_H
#define INCLUDE_SREC_INPUT_FILTER_FILL_H


#include <lib/interval.h>
#include <lib/srec/input/filter.h>
#include <lib/srec/record.h>

/**
  * The srec_input_filter_fill class is used to represent a filter
  * which replaces in set data locations with constant data.
  */
class srec_input_filter_fill:
    public srec_input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_filter_fill();

    /**
      * The constructor.
      */
    srec_input_filter_fill(srec_input *, int, const interval &);

    // See base class for documentation.
    virtual int read(srec_record &);

private:
    /**
      * The filler_value instance variable is used to remember the value
      * to assign to fill bytes.
      */
    int filler_value;

    /**
      * The filler_block instance variable is used to remember the base
      * of a dynamically allocated array of bytes, used to construct
      * filler block records.
      */
    unsigned char *filler_block;

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
    srec_input_filter_fill();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_filter_fill(const srec_input_filter_fill &);

    /**
      * The assignment.  Do not use.
      */
    srec_input_filter_fill &operator=(const srec_input_filter_fill &);
};

#endif // INCLUDE_SREC_INPUT_FILTER_FILL_H
