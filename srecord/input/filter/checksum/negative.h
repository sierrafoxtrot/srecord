//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2002, 2005-2008, 2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_INPUT_FILTER_CHECKSUM_NEGATIVE_H
#define SRECORD_INPUT_FILTER_CHECKSUM_NEGATIVE_H

#include <srecord/input/filter/checksum.h>

namespace srecord {

/**
  * The srecord::input_filter_checksum_negative class is used to represent the
  * state of a checksum filter that inserts a negative sum into the data.
  */
class input_filter_checksum_negative:
    public input_filter_checksum
{
public:
    /**
      * The destructor.
      */
    ~input_filter_checksum_negative() override;

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The deeper input source being checksummed.
      * @param address
      *     The address to place the checksum.
      * @param length
      *     The number of bytes of checksum to be placed into the result.
      * @param end
      *     The byte order.
      * @param width
      *     The width of the values being summed.  Usually 1 byte, but
      *     wider combinations are possible.  If you use something
      *     wider, it is assumed that they are aligned on multiples of
      *     that width, no provision for an offset is provided.
      */
    input_filter_checksum_negative(const input::pointer &a1,
        int a2, int a3, endian_t end, int a5 = 1);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The incoming data source to be filtered
      * @param address
      *     The address to place the checksum.
      * @param length
      *     The number of bytes of checksum to be placed into the result.
      * @param end
      *     The byte order.
      * @param width
      *     The width of the values being summed.  Usually 1 byte, but
      *     wider combinations are possible.  If you use something
      *     wider, it is assumed that they are aligned on multiples of
      *     that width, no provision for an offset is provided.
      */
    static auto create(const input::pointer &deeper, int a2, int a3,
        endian_t end, int a5 = 1) -> pointer;

protected:
    // See base class for document.
    auto calculate() -> sum_t override;

private:
    /**
      * The default constructor.  Do not use.
      */
    input_filter_checksum_negative() = delete;

    /**
      * The copy constructor.  Do not use.
      */
    input_filter_checksum_negative(const input_filter_checksum_negative &) = delete;

    /**
      * The assignment operator.  Do not use.
      */
    auto operator=(
        const input_filter_checksum_negative &) -> input_filter_checksum_negative & = delete;
};

};

#endif // SRECORD_INPUT_FILTER_CHECKSUM_NEGATIVE_H
