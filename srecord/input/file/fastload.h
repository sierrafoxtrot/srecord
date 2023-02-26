//
// srecord - manipulate eprom load files
// Copyright (C) 2001, 2003, 2006-2008, 2010, 2011, 2013 Peter Miller
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

#ifndef SRECORD_INPUT_FILE_FASTLOAD_H
#define SRECORD_INPUT_FILE_FASTLOAD_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The fastload class is used to parse an LSI Logic Fast Load format file.
  */
class input_file_fastload:
    public input_file
{
public:
    /**
      * The destructor.
      */
    ~input_file_fastload() override = default;

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
      * A constructor.  The input is read from the named file (or
      * the standard input if the file name is "-").
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_fastload(const std::string &file_name);

    /**
      * Read one line (record) of input.  Called by the #read method.
      * Returns false at end of file.
      */
    bool read_inner(record &);

    /**
      * Get a single base-64 digit.  Returns 0..63 for a valid digit.
      * Fatal error if not a valid base-64 digit.
      */
    int get_digit();

    /**
      * The get_number method is used to get a base-64 number.
      * The ordering is big-endian, but like ordinary decimal numbers.
      * Four digits is 24 bits.
      */
    uint32_t get_number(int min_digits, int max_digits);

    /**
      * The seen_some_input instance variable is used to
      * remember whether any input has been seen.
      */
    bool seen_some_input{false};

    /**
      * The address instance variable is used to represent the
      * current address within the file.  It is set by the /A command,
      * and advanced by the data, /B and /Z commands.
      */
    uint32_t address{0};

    /**
      * The expect_white_space method is used to ensure that while
      * space follows the various commands.  It doesn't consume any
      * input, since it uses peek_char to do the checking.
      */
    void expect_white_space();

public:
    /**
      * The default constructor.
      */
    input_file_fastload() = delete;

    /**
      * The copy constructor.
      */
    input_file_fastload(const input_file_fastload &) = delete;

    /**
      * The assignment operator.
      */
    input_file_fastload &operator=(const input_file_fastload &) = delete;
};

};

#endif // SRECORD_INPUT_FILE_FASTLOAD_H
