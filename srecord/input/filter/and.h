//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001, 2002, 2005-2008, 2010 Peter Miller
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

#ifndef SRECORD_INPUT_FILTER_AND_H
#define SRECORD_INPUT_FILTER_AND_H

#include <srecord/input/filter.h>

namespace srecord {

/**
  * The srecord::input_filter_and class is used to represent the input state
  * of a filter which bit-wise ANDs all incoming data bytes with a fixed
  * value.
  */
class input_filter_and:
    public input_filter
{
public:
    /**
      * The destructor.
      */
    ~input_filter_and() override;

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The deeper input to be used as a data source.
      * @param mask
      *     The value to be bit-wise ANDed with each incoming data byte.
      */
    input_filter_and(const input::pointer &deeper, int mask);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The incoming data source to be filtered
      * @param mask
      *     The value to be bit-wise ANDed with each incoming data byte.
      */
    static auto create(const input::pointer &deeper, int mask) -> pointer;

protected:
    // See base class for documentation.
    auto read(record &record) -> bool override;

private:
    /**
      * The value instance variable is used to remember the value to be
      * bit-wise ANDed with each incoming data byte.
      */
    int value;

    /**
      * The default constructor.  Do not use.
      */
    input_filter_and() = delete;

    /**
      * The copy constructor.  Do not use.
      */
    input_filter_and(const input_filter_and &) = delete;

    /**
      * The assignment operator.  Do not use.
      */
    auto operator=(const input_filter_and &) -> input_filter_and & = delete;
};

};

#endif // SRECORD_INPUT_FILTER_AND_H
