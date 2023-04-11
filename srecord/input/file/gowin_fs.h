//
// srecord - manipulate eprom load files
// Copyright (C) 2023 Daniel Anselmi
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

#ifndef SRECORD_INPUT_GOWIN_FS_H
#define SRECORD_INPUT_GOWIN_FS_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_gowin_fs class is used to represent the parse
  * state when reading a gowin .fs file.
  */
class input_file_gowin_fs:
    public input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~input_file_gowin_fs() = default;

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
    input_file_gowin_fs() = delete;

    /**
      * The copy constructor.
      */
    input_file_gowin_fs(const input_file_gowin_fs &) = delete;

    /**
      * The assignment operator.  Do not use.
      */
    input_file_gowin_fs &operator=(const input_file_gowin_fs &) = delete;

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
    input_file_gowin_fs(const std::string &file_name);

    /**
      * The address instance variable is used to remember the current
      * address of the next data record.  This is set and advanced by
      * the #read method.
      */
    uint32_t address;

    /**
      * The done instance variable is used to remember that we
      * don't expect more input data.
      */
    bool done;

    /**
      * The get_byte_binary method is used to fetch a byte value from
      * the input. It calls get_binary_digit_value 8 times, and
      * assembles them big-endian (most significant bit first).
      */
    int get_byte_binary();

    /**
      * The get_binary_digit_value method is used to fetch one binary digit
      * from the input, via the get_char method. It returns a value from 0 to 1.
      * If there's currently not a binary digit in the input stream
      * it returns -1.
      */
    int get_binary_digit_value();

    /**
      * skip input until there is a newline in the input.
      */
    void skip_until_eol();
};

};

#endif // SRECORD_INPUT_GOWIN_FS_H
