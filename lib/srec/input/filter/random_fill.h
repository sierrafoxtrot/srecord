//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2001, 2002, 2004, 2006-2008 Peter Miller
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

#ifndef INCLUDE_SREC_INPUT_FILTER_RANDOM_FILL_H
#define INCLUDE_SREC_INPUT_FILTER_RANDOM_FILL_H

#include <lib/interval.h>
#include <lib/srec/input/filter.h>
#include <lib/srec/record.h>

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

private:
    /**
      * The constructor.
      */
    srec_input_filter_random_fill(const srec_input::pointer &deeper,
        const interval &range);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The incoming data source to be filtered
      */
    static pointer create(const srec_input::pointer &deeper,
        const interval &range);

protected:
    // See base class for documentation.
    bool read(srec_record &record);

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
    bool generate(srec_record &record);

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
