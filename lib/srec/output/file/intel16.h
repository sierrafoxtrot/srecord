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

#ifndef INCLUDE_SREC_OUTPUT_FILE_INTEL16_H
#define INCLUDE_SREC_OUTPUT_FILE_INTEL16_H


#include <lib/srec/output/file.h>

/**
  * The srec_output_file_intel16 class is used to write an EPROM load file
  * in INHX16 format.
  */
class srec_output_file_intel16:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_intel16();

    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    srec_output_file_intel16(const std::string &file_name);

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
      * The write_inner method is used toi write a single line to the
      * output file.
      */
    void write_inner(int, unsigned long, const void *, int);

    /**
      * The address_base instance variable is used to remember the
      * current position within the output file
      */
    unsigned long address_base;

    /**
      * The pref_block_size instance variable is used to remember the
      * preferred number of bytes on each line of the output file.
      */
    int pref_block_size;

    /**
      * The default constructor.  Do not use.
      */
    srec_output_file_intel16();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_intel16(const srec_output_file_intel16 &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_intel16 &operator=(const srec_output_file_intel16 &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_INTEL16_H
