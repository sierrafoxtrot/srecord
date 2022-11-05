//
// srecord - Manipulate EPROM load files
// Copyright (C) 2011, 2013 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_INPUT_FILE_HEXDUMP_H
#define SRECORD_INPUT_FILE_HEXDUMP_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The input_file_hexdump class is used to represent the processing
  * required to read in a hexadecimal dump.
  */
class input_file_hexdump:
    public input_file
{
public:
    /**
      * The destructor.
      */
    ~input_file_hexdump() override;

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param filename
      *     The name of the file to be read.
      */
    static auto create(const std::string &filename) -> pointer;

protected:
    // See base class for documentation.
    auto read(class record &result) -> bool override;

    // See base class for documentation.
    auto get_file_format_name() const -> const char * override;

    // See base class for documentation.
    auto format_option_number() const -> int override;

private:
    /**
      * The constructor.
      *
      * @param filename
      *     The name of the file to be read.
      */
    input_file_hexdump(const std::string &filename);

    enum token_t
    {
        token_eof,
        token_byte,
        token_colon,
        token_eoln,
        token_junk
    };

    /**
      * The get_next_token method is used to read the next lexical token
      * from the input.
      *
      * It will set the #current_token_value instance variable for token_byte
      */
    auto get_next_token() -> token_t;

    /**
      * The discard_rest_of_line method is used to
      * discard all characters until the next end of line character.
      */
    auto discard_rest_of_line() -> bool;

    /**
      * The address instance variable is used to remember the current
      * address of the next data record.  This is set and advanced by
      * the #read method.
      */
    unsigned long address;

    /**
      * The data_seen instance variable is used to remember whether or
      * not any data has been seen in the file.  This is used to issue
      * an error when there is apparently no data in the file, and this
      * helps #guess to figure out the file is not of this type.
      */
    bool data_seen;

    /**
      * The current_token_value instance variable is used to remember
      * the value of the most recent token_byte seen.
      */
    unsigned current_token_value{};

    /**
      * The default constructor.  Do not use.
      */
    input_file_hexdump() = delete;

    /**
      * The copy constructor.  Do not use.
      */
    input_file_hexdump(const input_file_hexdump &) = delete;

    /**
      * The assignment operator.  Do not use.
      */
    auto operator=(const input_file_hexdump &) -> input_file_hexdump & = delete;
};

};

#endif // SRECORD_INPUT_FILE_HEXDUMP_H
