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

#ifndef INCLUDE_SREC_INPUT_FILTER_MAXIMUM_H
#define INCLUDE_SREC_INPUT_FILTER_MAXIMUM_H

#include <lib/endian.h>
#include <lib/srec/input/filter.h>

/**
  * The srec_input_filter_maximum class is used to represent an input
  * stream, which inserts the maximum address of the data into the data.
  */
class srec_input_filter_maximum:
    public srec_input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_filter_maximum();

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The input source to be filtered.
      * @param address
      *     The address at which to place the length.
      * @param length
      *     The number of bytes of checksum to be inserted.
      * @param end
      *     The byte order.
      */
    srec_input_filter_maximum(const srec_input::pointer &deeper, int address,
        int length, endian_t end);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The incoming data source to be filtered
      * @param address
      *     The address at which to place the length.
      * @param length
      *     The number of bytes of checksum to be inserted.
      * @param order
      *     The byte order.
      */
    static pointer create(const srec_input::pointer &deeper, int address,
        int length, endian_t end);

protected:
    // See base class for documentation.
    bool read(srec_record &record);

private:
    /**
      * The maximum_address instance variable is used to remember where
      * the maximum is to be placed.
      */
    int maximum_address;

    /**
      * The maximum_length instance variable is used to remember how many
      * bytes of maximum are to be inserted.
      */
    int maximum_length;

    /**
      * The end instance variable is used to remember the byte order of
      * the inserted maximum.
      */
    endian_t end;

    /**
      * The maximum instance variable is used to remember the maximum
      * address used in the incoming data stream.
      */
    unsigned long maximum;

    /**
      * The maximum_set instance variable is used to remember whether or
      * not the "maximum" instance variable has a useful values.  If you
      * get to the end of the deeper input and this is still false, it
      * means there was no input data.
      */
    bool maximum_set;

    /**
      * The generator method is used to generate the data record holding
      * the generated maximum to be inserted at the end of the data
      * stream.
      */
    bool generate(srec_record &record);

    /**
      * The default constructor.  Do not use.
      */
    srec_input_filter_maximum();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_filter_maximum(const srec_input_filter_maximum &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_filter_maximum &operator=(const srec_input_filter_maximum &);
};

#endif // INCLUDE_SREC_INPUT_FILTER_MAXIMUM_H
