//
//      srecord - The "srecord" program.
//      Copyright (C) 2007, 2008, 2010 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_BRECORD_H
#define SRECORD_OUTPUT_FILE_BRECORD_H

#include <srecord/output/file.h>

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

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to write the output to.  The string "-"
      *     is understood to mean the standard output.
      */
    srec_output_file_brecord(const std::string &file_name);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The file name to open to write data to.  The name "-" is
      *     understood to mean the standard output.
      */
    static pointer create(const std::string &file_name);

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
      * The default constructor.  Do not use.
      */
    srec_output_file_brecord();

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
#endif // SRECORD_OUTPUT_FILE_BRECORD_H
