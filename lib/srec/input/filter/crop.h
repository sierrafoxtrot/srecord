//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2001, 2002, 2005-2008 Peter Miller
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

#ifndef LIB_SREC_INPUT_FILTER_CROP_H
#define LIB_SREC_INPUT_FILTER_CROP_H

#include <lib/interval.h>
#include <lib/srec/input/filter.h>
#include <lib/srec/record.h>

/**
  * The srec_input_filter_crop filter is used to crop the data by
  * selecting portions of the address range to be passed through.
  */
class srec_input_filter_crop:
    public srec_input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_filter_crop();

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The input source to be cropped.
      * @param range
      *     The address range to be preserved.  The rest will be
      *     ignored.
      */
    srec_input_filter_crop(const srec_input::pointer &deeper,
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
      * The range instance variable is used to remember the address
      * range to be preserved.
      */
    interval range;

    /**
      * The dat instance variable is used to remember the current input
      * data record being filtered.
      */
    srec_record data;

    /**
      * The data_range instance variable is used to remember the address
      * range in the incoming data.
      */
    interval data_range;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_filter_crop();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_filter_crop(const srec_input_filter_crop &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_filter_crop &operator=(const srec_input_filter_crop &);
};

#endif // LIB_SREC_INPUT_FILTER_CROP_H
