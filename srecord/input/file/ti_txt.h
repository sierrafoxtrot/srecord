//
// srecord - manipulate eprom load files
// Copyright (C) 2007, 2008, 2010, 2011, 2013 Peter Miller
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

#ifndef SRECORD_INPUT_FILE_ti_txt_H
#define SRECORD_INPUT_FILE_ti_txt_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_ti_txt class is used to represent the parse
  * state when reading a ti_txt (MSP430) format input file.
  */
class input_file_ti_txt:
    public input_file
{
public:
    /**
      * The destructor.
      */
    ~input_file_ti_txt() override = default;

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
    input_file_ti_txt(const std::string &file_name);

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

    enum token_t
    {
        token_start_up,
        token_at,
        token_end_of_file,
        token_junk,
        token_number,
        token_q
    };

    /**
      * the token instance variable is used to remember the type of the
      * most recent token, as determined by the get_next_token method.
      */
    token_t token{token_start_up};

    /**
      * the token_value instance variable is used to remember the value
      * of the most recent token, as determined by the get_next_token
      * method.  Only meaningful for token_number, zero otherwise.
      */
    uint32_t token_value{0};

    /**
      * The address_warning instance variable is used to remember
      * whether or not we have already warned about addresses which are
      * too large.
      */
    bool address_warning{false};

    /**
      * The get_next_token method is used to partition the input into
      * the next symbol.  All the digits of a hexadecimal number are
      * considered a single symbol.
      */
    void get_next_token();

public:
    /**
      * The default constructor.
      */
    input_file_ti_txt() = delete;

    /**
      * The copy constructor.
      */
    input_file_ti_txt(const input_file_ti_txt &) = delete;

    /**
      * The assignment operator.
      */
    input_file_ti_txt &operator=(const input_file_ti_txt &) = delete;
};

};

#endif // SRECORD_INPUT_FILE_ti_txt_H
