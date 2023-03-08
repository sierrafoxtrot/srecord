//
// srecord - The "srecord" program.
// Copyright (C) 2007, 2008, 2010, 2011 Peter Miller
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

#ifndef SRECORD_INPUT_GENERATOR_REPEAT_H
#define SRECORD_INPUT_GENERATOR_REPEAT_H

#include <srecord/input/generator.h>

namespace srecord
{

/**
  * The srecord::input_generator_repeat class is used to represent
  * generating data which cycles over a fixed set of byte values.
  */
class input_generator_repeat:
    public input_generator
{
public:
    /**
      * The destructor.
      */
    ~input_generator_repeat() override;

private:
    /**
      * The constructor.
      *
      * @param range
      *     The address range over which to generate data.
      * @param data
      *     Pointer to the base of an array of data to be repeated.
      * @param length
      *     The length of the array of data to be repeated.
      */
    input_generator_repeat(const interval &range, const uint8_t *data,
        size_t length);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param range
      *     The address range over which to generate data.
      * @param data
      *     Pointer to the base of an array of data to be repeated.
      * @param length
      *     The length of the array of data to be repeated.
      */
    static pointer create(const interval &range, uint8_t *data,
        size_t length);

protected:
    // See base class for documentation.
    uint8_t generate_data(uint32_t address) override;

    // See base class for documentation.
    std::string filename() const override;

    // See base class for documentation.
    const char *get_file_format_name() const override;

private:
    /**
      * The address instance variable is used to remember the start of
      * the generated data, so that modulo arithmetic will align the
      * data repeats.
      */
    uint32_t address;

    /**
      * The data instance variable is used to remember the base of a
      * dynamically allocated array of data to be repeated.
      */
    uint8_t *data;

    /**
      * The length instance variable is used to remember the length of
      * the dynamically allocated array of data to be repeated.
      */
    size_t length;

public:
    /**
      * The default constructor.
      */
    input_generator_repeat() = delete;

    /**
      * The copy constructor.
      */
    input_generator_repeat(const input_generator_repeat &) = delete;

    /**
      * The assignment operator.
      */
    input_generator_repeat &operator=(const input_generator_repeat &) = delete;
};

};

#endif // SRECORD_INPUT_GENERATOR_REPEAT_H
