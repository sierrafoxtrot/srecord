//
//      srecord - The "srecord" program.
//      Copyright (C) 2007 Peter Miller
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
//      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//

#ifndef LIB_SREC_OUTPUT_FILE_BRECORD_H
#define LIB_SREC_OUTPUT_FILE_BRECORD_H

#include <lib/srec/output/file.h>

/**
  * The srec_output_file_brecord class is used to represent an output
  * files in Motorola MC68EZ328 bootstrap b-record format.
  */
class srec_output_file_brecord:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_brecord();

    /**
      * The default constructor.
      * Output will be written to the standard output.
      */
    srec_output_file_brecord();

    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to write the output to.  The string "-"
      *     is understood to mean the standard output.
      */
    srec_output_file_brecord(const std::string &file_name);

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
    enum { BUFFER_MAXIMUM_MAXIMUM = 31 };
    unsigned long buffer_address;
    unsigned buffer_length;
    unsigned buffer_maximum;
    unsigned char buffer[BUFFER_MAXIMUM_MAXIMUM];

    void flush();
    void buffer_stash(unsigned long address, unsigned char data);

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_brecord(const srec_output_file_brecord &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_brecord &operator=(const srec_output_file_brecord &);
};

// vim:ts=8:sw=4:et
#endif // LIB_SREC_OUTPUT_FILE_BRECORD_H
