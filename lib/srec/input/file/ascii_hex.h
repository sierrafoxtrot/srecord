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
// MANIFEST: interface definition for include/srec/input/file/ascii_hex.cc
//

#ifndef INCLUDE_SREC_INPUT_FILE_ASCII_HEX_H
#define INCLUDE_SREC_INPUT_FILE_ASCII_HEX_H


#include <lib/srec/input/file.h>

/**
  * The srec_input_file_ascii_hex class is used to repesent the parse
  * state when reading an Ascii-Hex format input file.
  */
class srec_input_file_ascii_hex:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_ascii_hex();

    /**
      * the constructor.
      */
    srec_input_file_ascii_hex(const char *filename);

    // See base class for documentation.
    int read(srec_record &);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    /**
      * The read_inner method is used to read a single record from
      * the file.  The read method calls it.
      */
    int read_inner(srec_record &);

    /**
      * The garbage_warning instance variable is used to remember whether
      * a warning has already been issued if the file contains garbage.
      */
    bool garbage_warning;

    /**
      * The seen_some_input instance variable is used to remember whether
      * any data has been seen in the input to date.
      */
    bool seen_some_input;

    /**
      * The address instance variable is used to remember where we are
      * up to in the input file, so it may be associated with data bytes.
      */
    unsigned long address;

    /**
      * The state instance variable is used to remember what state the
      * input is in at present.
      */
    enum { state_initial, state_body, state_ignore } state;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_ascii_hex();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_ascii_hex(const srec_input_file_ascii_hex &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_ascii_hex &operator=(const srec_input_file_ascii_hex &);
};

#endif // INCLUDE_SREC_INPUT_FILE_ASCII_HEX_H
