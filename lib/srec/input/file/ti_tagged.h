//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000, 2002, 2003, 2006, 2007 Peter Miller
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
// MANIFEST: interface definition for lib/srec/input/file/ti_tagged.cc
//

#ifndef INCLUDE_SREC_INPUT_FILE_TI_TAGGED_H
#define INCLUDE_SREC_INPUT_FILE_TI_TAGGED_H


#include <lib/srec/input/file.h>

/**
  * The srec_input_file_ti_tagged class is used to represent the parse
  * state of an input file in TI Tagged format.
  */
class srec_input_file_ti_tagged:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_ti_tagged();

    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    srec_input_file_ti_tagged(const string &file_name);

    // See base class for documentation.
    int read(srec_record &);

    // See base class for documentation.
    const char *get_file_format_name() const;

protected:
    /**
      * The get_char method is used to get a character from the input.
      * We override bacsie the checksum is character based, not byte
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
    srec_input_file_ti_tagged();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_ti_tagged(const srec_input_file_ti_tagged &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_ti_tagged &operator=(const srec_input_file_ti_tagged &);
};

#endif // INCLUDE_SREC_INPUT_FILE_TI_TAGGED_H
