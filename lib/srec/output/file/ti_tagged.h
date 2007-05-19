//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000-2003, 2006, 2007 Peter Miller
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

#ifndef INCLUDE_SREC_OUTPUT_FILE_TI_TAGGED_H
#define INCLUDE_SREC_OUTPUT_FILE_TI_TAGGED_H


#include <lib/srec/output/file.h>

/**
  * The srec_output_file_ti_tagged class is used to represent the output
  * state of a file in Texas Instruments Tagged format.
  */
class srec_output_file_ti_tagged:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_ti_tagged();

    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    srec_output_file_ti_tagged(const std::string &file_name);

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
    void put_char(int);

    // See base class for documentation.
    const char *format_name() const;

private:
    typedef srec_output_file inherited;

    /**
      * The address instance variable is used to remember the current
      * address within the output file.
      */
    unsigned long address;

    /**
      * The column instance variable is used to remember the current
      * text column within the output file.  This is so that we can
      * decide when to throw a new line.
      */
    int column;

    /**
      * The line_length instance variable is used to remember the maximum
      * permitted text line length.  This is so that we can decide when
      * to throw a new line.
      */
    int line_length;

    /**
      * The csum instance variable is used to remember the 16-bit running
      * total of the bytes emitted so far.  It gets reset by put_eoln.
      */
    int csum;

    /**
      * The put_eoln method is used to output the line termination,
      * which includes a checksum.
      */
    void put_eoln();

    /**
      * The default constructor.  Do not use.
      */
    srec_output_file_ti_tagged();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_ti_tagged(const srec_output_file_ti_tagged &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_ti_tagged &operator=(const srec_output_file_ti_tagged &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_TI_TAGGED_H
