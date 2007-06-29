//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2001, 2002, 2005-2007 Peter Miller
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
// MANIFEST: interface definition for lib/srec/input/filter/length.cc
//

#ifndef INCLUDE_SREC_INPUT_FILTER_LENGTH_H
#define INCLUDE_SREC_INPUT_FILTER_LENGTH_H


#include <lib/srec/input/filter.h>

/**
  * The srec_input_filter_length class is used to represent an input
  * stream, which inserts the length of the data into the data.
  */
class srec_input_filter_length:
    public srec_input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_filter_length();

    /**
      * The constructor.
      *
      * @param deeper
      *     The input source to be filtered.
      * @param address
      *     The address at which to place the length.
      * @param length
      *     The number of bytes of checksum to be inserted.
      * @param order
      *     The byte order: false => bigendian, true => little endian.
      */
    srec_input_filter_length(srec_input *deeper, int address, int length,
        int order);

    // See base class for documentation.
    virtual int read(srec_record &);

private:
    /**
      * The length_address instance variable is used to remember where
      * the length is to be placed.
      */
    int length_address;

    /**
      * The length_length instance variable is used to remember how many
      * bytes of length are to be inserted.
      */
    int length_length;

    /**
      * The length_order instance variable is used to remember the byte
      * order of the inserted length, false => big endian, true => little
      * endian.
      */
    int length_order;

    /**
      * The minimum instance variable is used to remember the minimum
      * address used in the incoming data stream.
      */
    unsigned long minimum;

    /**
      * The maximum instance variable is used to remember the maximum
      * address used in the incoming data stream.
      */
    unsigned long maximum;

    /**
      * The limits_set instance variable is used to remember whether or
      * not the "minimum" and "maximum" instance variable have useful
      * values.  If you get to the end of the deeper input and this is
      * still false, it means there was no input data.
      */
    bool limits_set;

    /**
      * The generator method is used to generate the data record holding
      * the generated length to be inserted at the end of the data stream.
      */
    int generate(srec_record &);

    /**
      * The default constructor.  Do not use.
      */
    srec_input_filter_length();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_filter_length(const srec_input_filter_length &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_filter_length &operator=(const srec_input_filter_length &);
};

#endif // INCLUDE_SREC_INPUT_FILTER_LENGTH_H
