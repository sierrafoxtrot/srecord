//
//      srecord - manipulate eprom load files
//      Copyright (C) 2003, 2006, 2007 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
//      02110-1301 USA.
//

#ifndef INCLUDE_SREC_OUTPUT_FILE_FORMATTED_BINARY_H
#define INCLUDE_SREC_OUTPUT_FILE_FORMATTED_BINARY_H


#include <lib/srec/output/file.h>

/**
  * The srec_output_file_formatted_binary class is used to represent
  * the current state when writing a file in the Formatted Binary format.
  */
class srec_output_file_formatted_binary:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_formatted_binary();

    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    srec_output_file_formatted_binary(const std::string &file_name);

protected:
    // See base class for documentation.
    void notify_upper_bound(long unsigned);

    // See base class for documentation.
    void write(const srec_record &);

    // See base class for documentation.
    void line_length_set(int);

    // See base class for documentation.
    void address_length_set(int);

    // See base class for documentation.
    int preferred_block_size_get() const;

    // See base class for documentation.
    const char *mode() const;

    // See base class for documentation.
    const char *format_name() const;

private:
    /**
      * The upper_bound instance variable is used to remember the upper
      * bound of memory addresses (maximum address plus one).
      */
    unsigned long upper_bound;

    /**
      * The address instance variable is used to remember the current
      * output address, to know if padding is required.
      */
    unsigned long address;

    /**
      * The check_sum instance variable is used to remember the running
      * sum of data baytes written to date.
      */
    unsigned short check_sum;

    /**
      * The default constructor.  Do not use.
      */
    srec_output_file_formatted_binary();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_formatted_binary(
        const srec_output_file_formatted_binary &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_formatted_binary &operator=(
        const srec_output_file_formatted_binary &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_FORMATTED_BINARY_H
