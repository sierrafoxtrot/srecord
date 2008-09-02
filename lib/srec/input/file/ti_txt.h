//
//      srecord - manipulate eprom load files
//      Copyright (C) 2007, 2008 Peter Miller
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

#ifndef LIB_SREC_INPUT_FILE_ti_txt_H
#define LIB_SREC_INPUT_FILE_ti_txt_H

#include <lib/srec/input/file.h>

/**
  * The srec_input_file_ti_txt class is used to repesent the parse
  * state when reading a ti_txt (MSP430) format input file.
  */
class srec_input_file_ti_txt:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_ti_txt();

private:
    /**
      * the constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    srec_input_file_ti_txt(const std::string &file_name);

public:
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
    bool read(srec_record &record);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    /**
      * The read_inner method is used to read a single record from
      * the file.  The read method calls it.
      */
    int read_inner(srec_record &);

    /**
      * The garbage_warning instance variable is used to remember whether
      * a warning has already been issued if the file contains garbage.
      */
    bool garbage_warning;

    /**
      * The seen_some_input instance variable is used to remember whether
      * any data has been seen in the input to date.
      */
    bool seen_some_input;

    /**
      * The address instance variable is used to remember where we are
      * up to in the input file, so it may be associated with data bytes.
      */
    unsigned long address;

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
    token_t token;

    /**
      * the token_value instance variable is used to remember the value
      * of the most recent token, as determined by the get_next_token
      * method.  Only meaningful for token_number, zero otherwise.
      */
    unsigned long token_value;

    /**
      * The address_warning instance variable is used to remember
      * whether or not we have already warned about addresses which are
      * too large.
      */
    bool address_warning;

    /**
      * The get_next_token method is used to partition the input into
      * the next symbol.  All the digits of a hexadecimal number are
      * considered a single symbol.
      */
    void get_next_token();

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_ti_txt();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_ti_txt(const srec_input_file_ti_txt &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_ti_txt &operator=(const srec_input_file_ti_txt &);
};

#endif // LIB_SREC_INPUT_FILE_ti_txt_H
