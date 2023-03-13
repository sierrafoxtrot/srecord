//
// srecord - manipulate eprom load files
// Copyright (C) 2000, 2002, 2003, 2006-2008, 2010, 2011, 2013 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_INPUT_EFINIX_BIT_H
#define SRECORD_INPUT_EFINIX_BIT_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_efinix_bit class is used to represent the parse
  * state when reading an efinix .bit or .hex file.
  */
class input_file_efinix_bit:
    public input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~input_file_efinix_bit() = default;

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be read.
      * @returns
      *     smart pointer to new instance
      */
    static pointer create(const std::string &file_name);

    /**
      * The default constructor.
      */
    input_file_efinix_bit() = delete;

    /**
      * The copy constructor.
      */
    input_file_efinix_bit(const input_file_efinix_bit &) = delete;

    /**
      * The assignment operator.  Do not use.
      */
    input_file_efinix_bit &operator=(const input_file_efinix_bit &) = delete;
protected:
    // See base class for documentation.
    bool read(record &record);

    // See base class for documentation.
    const char *get_file_format_name() const;

    // See base class for documentation.
    int format_option_number() const;

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_efinix_bit(const std::string &file_name);

    unsigned long address;
    bool done;
};

};

#endif // SRECORD_INPUT_EFINIX_BIT_H
