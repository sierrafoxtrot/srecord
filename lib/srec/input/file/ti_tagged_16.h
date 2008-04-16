//
//      srecord - manipulate eprom load files
//      Copyright (C) 2007, 2008 Peter Miller
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

#ifndef INCLUDE_SREC_INPUT_FILE_TI_TAGGED_16_H
#define INCLUDE_SREC_INPUT_FILE_TI_TAGGED_16_H

#include <lib/srec/input/file.h>

/**
  * The srec_input_file_ti_tagged_16 class is used to represent the
  * parse state of an input file in Texas Instruments SDSMAC 320 format.
  */
class srec_input_file_ti_tagged_16:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_ti_tagged_16();

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    srec_input_file_ti_tagged_16(const string &file_name);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be read.
      * @returns
      *     smart pointer to new instance
      */
    static pointer create(const string &file_name);

protected:
    // See base class for documentation.
    bool read(srec_record &record);

    // See base class for documentation.
    const char *get_file_format_name() const;

    /**
      * The get_char method is used to get a character from the input.
      * We override because the checksum is character based, not byte
      * based.
      */
    int get_char();

private:
    typedef srec_input_file inherited;

    /**
      * The read_inner method is used to read a single line of input.
      * the raed method is a wrapper around this method.
      */
    int read_inner(srec_record &);

    /**
      * The address instance variable is used to remember where we are
      * up to in the input file.
      */
    unsigned long address;

    /**
      * The csum instance variable is used to remember the running
      * checksum.
      */
    int csum;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_ti_tagged_16();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_ti_tagged_16(const srec_input_file_ti_tagged_16 &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_ti_tagged_16 &operator=(
        const srec_input_file_ti_tagged_16 &);
};

#endif // INCLUDE_SREC_INPUT_FILE_TI_TAGGED_16_H
