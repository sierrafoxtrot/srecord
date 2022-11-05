//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001, 2002, 2004, 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_INPUT_FILTER_RANDOM_FILL_H
#define SRECORD_INPUT_FILTER_RANDOM_FILL_H

#include <srecord/interval.h>
#include <srecord/input/filter.h>
#include <srecord/record.h>

namespace srecord
{

/**
  * The srecord::input_filter_random_fill class is used to represent a filter
  * which replaces in set data locations with random data.
  */
class input_filter_random_fill:
    public input_filter
{
public:
    /**
      * The destructor.
      */
    ~input_filter_random_fill() override;

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The incoming data source to be filtered
      * @param range
      *     the address range to be filled.
      */
    input_filter_random_fill(const input::pointer &a1,
        const interval &a3);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The incoming data source to be filtered
      * @param range
      *     the address range to be filled.
      */
    static auto create(const input::pointer &deeper, const interval &range) -> pointer;

protected:
    // See base class for documentation.
    auto read(record &record) -> bool override;

private:
    /**
      * The range instance variable is used to remember the range of
      * addresses to be filled.  As fill blocks are produced the range
      * is reduced.
      */
    interval range;

    /**
      * The generate method is used to generate fill records.
      */
    auto generate(record &record) -> bool;

    /**
      * The default constructor.  Do not use.
      */
    input_filter_random_fill() = delete;

    /**
      * The copy constructor.  Do not use.
      */
    input_filter_random_fill(const input_filter_random_fill &) = delete;

    /**
      * The assignment operator.  Do not use.
      */
    auto operator=(const input_filter_random_fill &) -> input_filter_random_fill & = delete;
};

};

#endif // SRECORD_INPUT_FILTER_RANDOM_FILL_H
