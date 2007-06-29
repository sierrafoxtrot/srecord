//
//      srecord - manipulate eprom load files
//      Copyright (C) 2001, 2002, 2006, 2007 Peter Miller
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

#ifndef INCLUDE_SREC_OUTPUT_FILE_ATMEL_GENERIC_H
#define INCLUDE_SREC_OUTPUT_FILE_ATMEL_GENERIC_H


#include <lib/srec/output/file.h>

/**
  * The srec_output_file_ascii_hex class is used to write a file in
  * Atmel Generic format.
  */
class srec_output_file_atmel_generic:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_atmel_generic();

    /**
      * The default constructor.
      * Output will be written to the standard output.
      */
    srec_output_file_atmel_generic();

    /**
      * The constructor.
      *
      * @param file_name
      *     The file to write the data to, or "-" to mean the standard
      *     output.
      * @param bigend
      *     The byte order of the output, true means big endian, and
      *     false means little endian.
      */
    srec_output_file_atmel_generic(const std::string &file_name, bool bigend);

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
    const char *format_name() const;

private:
    /**
      * The bugend instance variable is used to remember the byte order
      * of the output, true means big endian, and false means little
      * endian.
      */
    bool bigend;

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_atmel_generic(const srec_output_file_atmel_generic &);

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_atmel_generic &operator=(
        const srec_output_file_atmel_generic &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_ATMEL_GENERIC_H
