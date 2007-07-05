//
//      srecord - The "srecord" program.
//      Copyright (C) 2007 Peter Miller
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

#ifndef LIB_SREC_INPUT_GENERATOR_RANDOM_H
#define LIB_SREC_INPUT_GENERATOR_RANDOM_H

#include <lib/srec/input/generator.h>

/**
  * The srec_input_generator_random class is used to represent a factory
  * which manufactures random data.
  */
class srec_input_generator_random:
    public srec_input_generator
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_generator_random();

    /**
      * The constructor.
      */
    srec_input_generator_random(const interval &range);

protected:
    // See base class for documentation.
    string filename() const;

    // See base class for documentation.
    const char *get_file_format_name() const;

    // See base class for documentation.
    unsigned char generate_data(unsigned long address);

private:
    /**
      * The default constructor.  Do not use.
      */
    srec_input_generator_random();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_generator_random(const srec_input_generator_random &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_generator_random &operator=(const srec_input_generator_random &);
};

// vim:ts=8:sw=4:et
#endif // LIB_SREC_INPUT_GENERATOR_RANDOM_H
