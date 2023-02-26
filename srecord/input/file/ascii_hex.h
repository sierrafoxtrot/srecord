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

#ifndef SRECORD_INPUT_FILE_ASCII_HEX_H
#define SRECORD_INPUT_FILE_ASCII_HEX_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_ascii_hex class is used to represent the parse
  * state when reading an Ascii-Hex format input file.
  */
class input_file_ascii_hex:
    public input_file
{
public:
    /**
      * The destructor.
      */
    ~input_file_ascii_hex() override = default;

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

protected:
    // See base class for documentation.
    bool read(record &record) override;

    // See base class for documentation.
    const char *get_file_format_name() const override;

    // See base class for documentation.
    int format_option_number() const override;

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_ascii_hex(const std::string &file_name);

    /**
      * The read_inner method is used to read a single record from
      * the file.  The read method calls it.
      */
    bool read_inner(record &);

    /**
      * The garbage_warning instance variable is used to remember whether
      * a warning has already been issued if the file contains garbage.
      */
    bool garbage_warning{false};

    /**
      * The seen_some_input instance variable is used to remember whether
      * any data has been seen in the input to date.
      */
    bool seen_some_input{false};

    /**
      * The address instance variable is used to remember where we are
      * up to in the input file, so it may be associated with data bytes.
      */
    uint32_t address{0};

    /**
      * The state instance variable is used to remember what state the
      * input is in at present.
      */
    enum { state_initial, state_body, state_ignore } state {state_initial};

public:
    /**
      * The default constructor.
      */
    input_file_ascii_hex() = delete;

    /**
      * The copy constructor.
      */
    input_file_ascii_hex(const input_file_ascii_hex &) = delete;

    /**
      * The assignment operator.
      */
    input_file_ascii_hex &operator=(const input_file_ascii_hex &) = delete;
};

};

#endif // SRECORD_INPUT_FILE_ASCII_HEX_H
