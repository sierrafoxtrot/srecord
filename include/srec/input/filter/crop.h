//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998, 1999, 2001, 2002, 2005, 2006 Peter Miller;
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
// MANIFEST: interface definition for lib/srec/input/filter/crop.cc
//

#ifndef INCLUDE_SREC_INPUT_FILTER_CROP_H
#define INCLUDE_SREC_INPUT_FILTER_CROP_H


#include <interval.h>
#include <srec/input/filter.h>
#include <srec/record.h>

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

    /**
      * The constructor.
      *
      * @param deeper
      *     The input source to be cropped.
      * @param range
      *     The address range to be preserved.  The rest will be
      *     ignored.
      */
    srec_input_filter_crop(srec_input *deeper, const interval &range);

    // See base class for documentation.
    virtual int read(srec_record &);

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

#endif // INCLUDE_SREC_INPUT_FILTER_CROP_H
