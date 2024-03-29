//
// srecord - Manipulate EPROM load files
// Copyright (C) 2008-2011 Peter Miller
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

#ifndef SRECORD_INPUT_FILTER_INTERVAL_MINIMUM_H
#define SRECORD_INPUT_FILTER_INTERVAL_MINIMUM_H

#include <srecord/input/filter/interval.h>

namespace srecord
{

/**
  * The srecord::input_filter_interval_minimum class is used to represent a
  * filter which injects the minimum address of the data into the data.
  */
class input_filter_interval_minimum:
    public input_filter_interval
{
public:
    /**
      * The destructor.
      */
    ~input_filter_interval_minimum() override = default;

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The input source to be filtered.
      * @param address
      *     The address at which to place the minimum address.
      * @param length
      *     The number of bytes of minimum address to be inserted.
      * @param end
      *     The byte order.
      * @param inclusive
      *     true if the output location is included in the address
      *     range, false if not
      */
    input_filter_interval_minimum(const pointer &deeper, long address,
        int length, endian_t end, bool inclusive);

public:
    /**
      * The constructor.
      *
      * @param deeper
      *     The input source to be filtered.
      * @param address
      *     The address at which to place the minimum address.
      * @param length
      *     The number of bytes of minimum address to be inserted.
      * @param end
      *     The byte order.
      * @param inclusive
      *     true if the output location is included in the address
      *     range, false if not
      */
    static pointer create(const pointer &deeper, long address, int length,
        endian_t end, bool inclusive);

protected:
    // See base class for documentation.
    long calculate_result() const override;

public:
    /**
      * The default constructor.
      */
    input_filter_interval_minimum() = delete;

    /**
      * The copy constructor.
      */
    input_filter_interval_minimum(
        const input_filter_interval_minimum &) = delete;

    /**
      * The assignment operator.
      */
    input_filter_interval_minimum &operator=(
        const input_filter_interval_minimum &) = delete;
};

};

#endif // SRECORD_INPUT_FILTER_INTERVAL_MINIMUM_H
