//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009-2011, 2013 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_INPUT_FILE_MIF_H
#define SRECORD_INPUT_FILE_MIF_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_mif class is used to represent the parse state
  * when reading a Memory Initialization File (MIF) formatted file.
  */
class input_file_mif:
    public input_file
{
public:
    /**
      * The destructor.
      */
    ~input_file_mif() override;

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be read.
      * @returns
      *     smart pointer to new instance
      */
    static auto create(const std::string &file_name) -> pointer;

protected:
    // See base class for documentation.
    auto read(record &record) -> bool override;

    // See base class for documentation.
    auto get_file_format_name() const -> const char * override;

    // See base class for documentation.
    auto format_option_number() const -> int override;

private:
    /**
      * The constructor.
      * It is private on purpose, use the #create class method instead.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_mif(const std::string &file_name);

    enum state_t
    {
        state_header,
        state_eof,
        state_address,
        state_data
    };

    /**
      * The state instance variable is used to remember the parse state
      * as we progress through the file.
      */
    state_t state;

    /**
      * The address instance variable is used to remember where we are
      * up to in the input file, so it may be associated with data bytes.
      */
    unsigned long address;

    /**
      * The lex_radix instance variable is used to remember the numeric
      * radix to be used when parsing numbers.
      */
    unsigned lex_radix;

    /**
      * The lex_addr_radix instance variable is used to remember the
      * numeric radix to be used when parsing address values.
      */
    unsigned lex_addr_radix;

    /**
      * The lex_data_radix instance variable is used to remember the
      * numeric radix to be used when parsing data values.
      */
    unsigned lex_data_radix;

    enum token_t
    {
        token_address_radix,
        token_begin,
        token_bin,
        token_bracket_left,
        token_bracket_right,
        token_colon,
        token_content,
        token_data_radix,
        token_dec,
        token_depth,
        token_dot,
        token_dotdot,
        token_end,
        token_eof,
        token_equals,
        token_hex,
        token_junk,
        token_minus,
        token_number,
        token_oct,
        token_semicolon,
        token_width
    };

    /**
      * The lex method is used to obtain the next lexical token from the
      * input stream.
      */
    auto lex() -> token_t;

    /**
      * The token_value instance variable is used to remember the
      * numeric value of token_number returns from #lex.
      */
    long token_value{};

    /**
      * The lex_addr method is used to obtain the next lexical token
      * from the input stream, using the address radix.
      */
    auto lex_addr() -> token_t;

    /**
      * The lex_data method is used to obtain the next lexical token
      * from the input stream, using the data radix.
      */
    auto lex_data() -> token_t;

    /**
      * The syntax_error method is used to report parse errors when
      * reading the file.
      *
      * @param text
      *     The text of the error message.
      */
    void syntax_error(const char *text);

    void get_equals();
    auto get_number() -> long;
    void get_semicolon();
    auto get_radix() -> int;
    void get_colon();

    unsigned width;
    unsigned width_in_bytes;

    /**
      * The default constructor.  Do not use.
      */
    input_file_mif() = delete;

    /**
      * The copy constructor.  Do not use.
      */
    input_file_mif(const input_file_mif &) = delete;

    /**
      * The assignment operator.  Do not use.
      */
    auto operator=(const input_file_mif &) -> input_file_mif & = delete;
};

};

#endif // SRECORD_INPUT_FILE_MIF_H
