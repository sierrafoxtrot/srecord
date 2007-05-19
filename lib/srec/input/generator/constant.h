//
//      srecord - The "srecord" program.
//      Copyright (C) 2007 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//

#ifndef LIB_SREC_INPUT_GENERATOR_CONSTANT_H
#define LIB_SREC_INPUT_GENERATOR_CONSTANT_H

#include <lib/srec/input/generator.h>

/**
  * The srec_input_generator_constant class is used to represent the
  * state of a data generator which supplies constant bytes.
  */
class srec_input_generator_constant:
    public srec_input_generator
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_generator_constant();

    /**
      * The constructor.
      */
    srec_input_generator_constant(const interval &range, unsigned char datum);

protected:
    // See base class for documentation.
    string filename() const;

    // See base class for documentation.
    const char *get_file_format_name() const;

    // See base class for documentation.
    unsigned char generate_data(unsigned long address);

private:
    /**
      * The datum instance variable is used to remember the constant
      * byte value to be generated.
      */
    unsigned char datum;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_generator_constant();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_generator_constant(const srec_input_generator_constant &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_generator_constant &operator=(
        const srec_input_generator_constant &);
};

// vim:ts=8:sw=4:et
#endif // LIB_SREC_INPUT_GENERATOR_CONSTANT_H