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

#ifndef INCLUDE_SREC_OUTPUT_FILE_COSMAC_H
#define INCLUDE_SREC_OUTPUT_FILE_COSMAC_H


#include <lib/srec/output/file.h>

/**
  * The srec_output_file_cosmac class is used to represent an output
  * which emits RCA Cosmac format.
  */
class srec_output_file_cosmac:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_cosmac();

    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    srec_output_file_cosmac(const std::string &file_name);

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
      * The address instance variable is used to remember the current
      * output address, so that file size can be optimized.
      */
    unsigned long address;

    /**
      * The address_length is used to remember the preferred number of
      * characters used to represent an address in the output file.
      * Defaults to 4 characters.
      */
    unsigned address_length;

    /**
      * The line_length instance variable is used to remember the maximum
      * line length of the output.  Defaults to 80 characters.
      */
    unsigned line_length;

    /**
      * The column instance variable is used to remember which output
      * column the current output position is on.
      */
    unsigned column;

    /**
      * The header_required instance variable is used to remember whether
      * or not the header (the !M command) has been issued, or not.
      */
    bool header_required;

    /**
      * The default constructor.  Do not use.
      */
    srec_output_file_cosmac();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_cosmac(const srec_output_file_cosmac &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_cosmac &operator=(const srec_output_file_cosmac &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_COSMAC_H
