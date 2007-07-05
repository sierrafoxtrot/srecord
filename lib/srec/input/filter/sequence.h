//
//      srecord - manipulate eprom load files
//      Copyright (C) 2006, 2007 Peter Miller
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
// MANIFEST: interface definition for lib/srec/input/filter/sequence.cc
//

#ifndef LIB_SREC_INPUT_FILTER_SEQUENCE_H
#define LIB_SREC_INPUT_FILTER_SEQUENCE_H

#include <lib/srec/input/filter.h>

/**
  * The srec_input_filter_sequence class is used to represent an input
  * filter which does not change the data in any way, but issues
  * warnings of the data is not instrictky ascending address order.
  */
class srec_input_filter_sequence:
    public srec_input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_filter_sequence();

    /**
      * The constructor.
      */
    srec_input_filter_sequence(srec_input *deeper);

protected:
    // See base class for documentation.
    int read(srec_record &record);

private:
    /**
      * The last_address instance variable is used to remember the high
      * water mark for data record addresses to date.  Records with data
      * addresses less than this cause an "out of sequence" warning.
      * It is check and updated by the read() method.
      */
    unsigned long last_address;

    /**
      * The warned instance variable is used to remember whether or not
      * an "out of sequence" warning has already been issued for this
      * file.
      *
      * We only issue a single warning, because the linker in some
      * embedded too chains emit zillions of these as they walk across
      * object files updating segemnts in parallel.
      */
    bool warned;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_filter_sequence();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_filter_sequence(const srec_input_filter_sequence &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_filter_sequence &operator=(const srec_input_filter_sequence &);
};

#endif // LIB_SREC_INPUT_FILTER_SEQUENCE_H
