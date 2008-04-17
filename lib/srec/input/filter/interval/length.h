//
//      srecord - Manipulate EPROM load files
//      Copyright (C) 2008 Peter Miller
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

#ifndef LIB_SREC_INPUT_FILTER_INTERVAL_LENGTH_H
#define LIB_SREC_INPUT_FILTER_INTERVAL_LENGTH_H

#include <lib/srec/input/filter/interval.h>

/**
  * The srec_input_filter_interval_length class is used to represent a
  * filter which injects the maximum minus minimum addresses of the data
  * into the data.
  */
class srec_input_filter_interval_length:
    public srec_input_filter_interval
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_filter_interval_length();

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
      * @param inclusive
      *     true if the output location is included in the address
      *     range, false if not
      */
    srec_input_filter_interval_length(const srec_input::pointer &deeper,
        long address, int length, endian_t end, bool inclusive);

public:
    /**
      * The create class method is used to create a new dynamically
      * allocated instance of this class.
      *
      * @param deeper
      *     The input source to be filtered.
      * @param address
      *     The address at which to place the length.
      * @param length
      *     The number of bytes of checksum to be inserted.
      * @param end
      *     The byte order.
      * @param inclusive
      *     true if the output location is included in the address
      *     range, false if not
      */
    static pointer create(const srec_input::pointer &deeper, long address,
        int length, endian_t end, bool inclusive);

protected:
    // See base class for documentation.
    long calculate_result() const;

private:
    /**
      * The default constructor.
      */
    srec_input_filter_interval_length();

    /**
      * The copy constructor.
      */
    srec_input_filter_interval_length(
        const srec_input_filter_interval_length &);

    /**
      * The assignment operator.
      */
    srec_input_filter_interval_length &operator=(
        const srec_input_filter_interval_length &);
};

// vim:ts=8:sw=4:et
#endif // LIB_SREC_INPUT_FILTER_INTERVAL_LENGTH_H
