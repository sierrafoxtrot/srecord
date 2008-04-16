//
//      srecord - manipulate eprom load files
//      Copyright (C) 2001, 2002, 2005-2008 Peter Miller
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

#ifndef INCLUDE_SREC_INPUT_FILTER_UNFILL_H
#define INCLUDE_SREC_INPUT_FILTER_UNFILL_H

#include <lib/srec/input/filter.h>
#include <lib/srec/record.h>

/**
  * The srec_input_filter_unfill class is used to represent a filter
  * which makes holes in the data wherever a specified data byte value
  * appears.
  *
  * This is the inverse of the srec_input_filter_fill class.
  *
  * Usually this is used to find the actual limits of data from an
  * extracted EPROM image; you can specify a minimum run legth of the
  * same byte, so that you don't simply get a 1/256 reduction in density
  * in the middle of "real" data.
  */
class srec_input_filter_unfill:
    public srec_input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_filter_unfill();

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The deeper input source to be filtered.
      * @param value
      *     The value of the bytes to be turned into holes.
      * @param minimum
      *     The minimum run length to be considered a hole.
      */
    srec_input_filter_unfill(const srec_input::pointer &deeper, int value,
        int minimum);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The incoming data source to be filtered
      * @param value
      *     The value of the bytes to be turned into holes.
      * @param minimum
      *     The minimum run length to be considered a hole.
      */
    static pointer create(const srec_input::pointer &deeper, int value,
        int minimum);

protected:
    // See base class for documentation.
    bool read(srec_record &record);

private:
    /**
      * The fill_value instance variable is used to remember the value
      * of the bytes to be turned into holes.
      */
    int fill_value;

    /**
      * The fill_minimum instance variable is used to remember the
      * minimum run length to be considered a hole.
      */
    int fill_minimum;

    /**
      * The buffer instance variable is used to remember the data
      * that has been read from the deeper input source and is being
      * processed to unfill certain valued bytes.
      */
    srec_record buffer;

    /**
      * The fill_value instance variable is used to remember where we
      * are up to in the "bufefr" instance variable.
      */
    int buffer_pos;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_filter_unfill();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_filter_unfill(const srec_input_filter_unfill &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_filter_unfill &operator=(const srec_input_filter_unfill &);
};

#endif // INCLUDE_SREC_INPUT_FILTER_UNFILL_H
