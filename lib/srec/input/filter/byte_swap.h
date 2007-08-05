//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000-2002, 2005-2007 Peter Miller
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

#ifndef INCLUDE_SREC_INPUT_FILTER_BYTE_SWAP_H
#define INCLUDE_SREC_INPUT_FILTER_BYTE_SWAP_H


#include <lib/srec/input/filter.h>
#include <lib/srec/record.h>

/**
  * The srec_input_filter_byte_swap class is used to reverse the
  * even-addressed and odd-addressed bytes in an input source.
  */
class srec_input_filter_byte_swap:
    public srec_input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_filter_byte_swap();

    /**
      * The constructor.
      *
      * @param deeper
      *     The incoming data sourec to be byte-swapped.
      */
    srec_input_filter_byte_swap(srec_input *deeper);

    // See base class for documentation.
    int read(srec_record &);

    // See base class for documentation.
    void command_line(srec_arglex *cmdln);

private:
    /**
      * The buffer instance variable is used to remember the data
      * fetched from the deeper data source.
      */
    srec_record buffer;

    /**
      * The buffer_pos instance variable is used to remeber the byte
      * position within the buffer instance variable.
      */
    int buffer_pos;

    /**
      * The mask instance variable is used to remember the bit mask to
      * be xor-ed with the address to form the byte-swapped address.
      * It defualt to 1, but can be altered by command line option.
      */
    unsigned mask;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_filter_byte_swap();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_filter_byte_swap(const srec_input_filter_byte_swap &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_filter_byte_swap &operator=(const srec_input_filter_byte_swap &);
};

#endif // INCLUDE_SREC_INPUT_FILTER_BYTE_SWAP_H
