//
// srecord - manipulate eprom load files
// Copyright (C) 2003, 2006-2008, 2010, 2011 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_OUTPUT_FILE_FORMATTED_BINARY_H
#define SRECORD_OUTPUT_FILE_FORMATTED_BINARY_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_formatted_binary class is used to represent
  * the current state when writing a file in the Formatted Binary format.
  */
class output_file_formatted_binary:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_formatted_binary() override;

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    output_file_formatted_binary(const std::string &file_name);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    static pointer create(const std::string &file_name);

protected:
    // See base class for documentation.
    void notify_upper_bound(uint32_t) override;

    // See base class for documentation.
    void write(const record &) override;

    // See base class for documentation.
    void line_length_set(int) override;

    // See base class for documentation.
    void address_length_set(int) override;

    // See base class for documentation.
    int preferred_block_size_get() const override;

    // See base class for documentation.
    bool preferred_block_size_set(int nbytes) override;

    // See base class for documentation.
    const char *format_name() const override;

    // See base class for documentation.
    bool is_binary() const override;

private:
    /**
      * The upper_bound instance variable is used to remember the upper
      * bound of memory addresses (maximum address plus one).
      */
    uint32_t upper_bound{0};

    /**
      * The address instance variable is used to remember the current
      * output address, to know if padding is required.
      */
    uint32_t address{0};

    /**
      * The check_sum instance variable is used to remember the running
      * sum of data bytes written to date.
      */
    uint16_t check_sum{0};

public:
    /**
      * The default constructor.
      */
    output_file_formatted_binary() = delete;

    /**
      * The copy constructor.
      */
    output_file_formatted_binary(const output_file_formatted_binary &) = delete;

    /**
      * The assignment operator.
      */
    output_file_formatted_binary &operator=(
        const output_file_formatted_binary &) = delete;
};

};

#endif // SRECORD_OUTPUT_FILE_FORMATTED_BINARY_H
