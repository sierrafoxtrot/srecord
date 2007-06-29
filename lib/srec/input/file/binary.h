//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998-2000, 2002, 2003, 2006, 2007 Peter Miller
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
// MANIFEST: interface definition for srec/input/file/binary.cc
//

#ifndef INCLUDE_SREC_INPUT_FILE_BINARY_H
#define INCLUDE_SREC_INPUT_FILE_BINARY_H


#include <lib/srec/input/file.h>

/**
  * The srec_input_file_binary class is used to represent the parse
  * state of a binary input stream.
  */
class srec_input_file_binary:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_binary();

    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    srec_input_file_binary(const string &file_name);

    // See nase class for documentation.
    int read(srec_record &);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    /**
      * The address instance variable is used to remember where we are
      * up to in the file.
      */
    unsigned long address;

    // See base class for documentation.
    const char *mode() const;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_binary();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_binary(const srec_input_file_binary &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_binary &operator=(const srec_input_file_binary &);
};

#endif // INCLUDE_SREC_INPUT_FILE_BINARY_H
