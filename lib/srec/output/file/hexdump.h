//
//      srecord - The "srecord" program.
//      Copyright (C) 2007 Peter Miller
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

#ifndef LIB_SREC_OUTPUT_FILE_HEXDUMP_H
#define LIB_SREC_OUTPUT_FILE_HEXDUMP_H

#include <lib/srec/output/file.h>

/**
  * The srec_output_file_hexdump class is used to represent an output
  * file which emits a hexadecimal dump (including ASCII) of the data.
  */
class srec_output_file_hexdump:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_hexdump();

    /**
      * The constructor.
      *
      * @param file_name
      *     The file name to open to write data to.  The name "-" is
      *     understood to mean the standard output.
      */
    srec_output_file_hexdump(const std::string &file_name);

protected:
    // See base class for documentation.
    void write(const srec_record &);

    // See base class for documentation.
    void line_length_set(int);

    // See base class for documentation.
    void address_length_set(int);

    // See base class for documentation.
    int preferred_block_size_get() const;

    // See base class for documentation.
    void command_line(srec_arglex *cmdln);

    // See base class for documentation.
    const char *format_name() const;

private:
    /**
      * The FUBAR instance variable
      * is used to remember
      */
    int number_of_columns;
    unsigned long row_cache_address;
    unsigned long row_cache_address_mask;
    size_t row_cache_size;
    char *row_cache;

    /**
      * The address_length instance variable is used to remember how
      * many bytes to emit when emitting addresses.
      */
    int address_length;

    /**
      * The emit_byte method is used to emit a single byte.  It uses
      * column to track the position, so as not to exceed line_length.
      */
    void emit_byte(unsigned long address, unsigned char data);

    /**
      * The row_cache_print method is used to print the row cache to the
      * file and then erase the cache in preparation for another row.
      */
    void row_cache_print();

    int columns_to_line_length(int cols);

    /**
      * The default constructor.  Do not use.
      */
    srec_output_file_hexdump();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_hexdump(const srec_output_file_hexdump &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_hexdump &operator=(const srec_output_file_hexdump &);
};

// vim:ts=8:sw=4:et
#endif // LIB_SREC_OUTPUT_FILE_HEXDUMP_H
