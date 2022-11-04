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

#ifndef SRECORD_INPUT_GENERATOR_RANDOM_H
#define SRECORD_INPUT_GENERATOR_RANDOM_H

#include <srecord/input/generator.h>

namespace srecord
{

/**
  * The srecord::input_generator_random class is used to represent a factory
  * which manufactures random data.
  */
class input_generator_random:
    public input_generator
{
public:
    /**
      * The destructor.
      */
    ~input_generator_random() override;

private:
    /**
      * The constructor.
      */
    input_generator_random(const interval &range);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static auto create(const interval &range) -> pointer;

protected:
    // See base class for documentation.
    auto filename() const -> std::string override;

    // See base class for documentation.
    auto get_file_format_name() const -> const char * override;

    // See base class for documentation.
    auto generate_data(unsigned long address) -> unsigned char override;

private:
    /**
      * The default constructor.  Do not use.
      */
    input_generator_random() = delete;

    /**
      * The copy constructor.  Do not use.
      */
    input_generator_random(const input_generator_random &) = delete;

    /**
      * The assignment operator.  Do not use.
      */
    auto operator=(const input_generator_random &) -> input_generator_random & = delete;
};

};

#endif // SRECORD_INPUT_GENERATOR_RANDOM_H
