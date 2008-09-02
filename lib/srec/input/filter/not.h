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

#ifndef LIB_SREC_INPUT_FILTER_NOT_H
#define LIB_SREC_INPUT_FILTER_NOT_H

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

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The input source to be filtered.
      */
    srec_input_filter_not(const srec_input::pointer &deeper);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The incoming data source
      */
    static pointer create(const srec_input::pointer &deeper);

protected:
    // See base class for documentation.
    bool read(srec_record &record);

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

#endif // LIB_SREC_INPUT_FILTER_NOT_H
