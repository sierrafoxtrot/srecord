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

#ifndef SRECORD_INPUT_FILE_IDT_H
#define SRECORD_INPUT_FILE_IDT_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_idt class is used to represent the parse
  * state of a IDT/sim binary format input file.
  */
class input_file_idt:
    public input_file
{
public:
    /**
      * The destructor.
      */
    ~input_file_idt() override = default;

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
    bool is_binary() const override;

    // See base class for documentation.
    int format_option_number() const override;

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_idt(const std::string &file_name);

    /**
      * The data_count instance variable is used to remember the number
      * of data lines has occurred so far in the input file.
      */
    uint32_t data_count{0};

    /**
      * The read_inner method is used to read a record of input.
      * The read method is a wrapper around this method.
      */
    bool read_inner(record &);

    /**
      * The seen_some_input instance variable is used to remember where
      * any data has been seen in this file yet.
      */
    bool seen_some_input{false};

    /**
      * The record_format_error is a wrapper around #fatal_error to
      * complain abut format errors.
      */
    void record_format_error();

public:
    /**
      * The default constructor.
      */
    input_file_idt() = delete;

    /**
      * The copy constructor.
      */
    input_file_idt(const input_file_idt &) = delete;

    /**
      * The assignment operator.
      */
    input_file_idt &operator=(const input_file_idt &) = delete;
};

};

#endif // SRECORD_INPUT_FILE_IDT_H
