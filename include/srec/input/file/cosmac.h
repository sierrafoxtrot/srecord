//
//	srecord - manipulate eprom load files
//	Copyright (C) 2003 Peter Miller;
//	All rights reserved.
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: interface definition for include/srec/input/file/cosmac.cc
//

#ifndef INCLUDE_SREC_INPUT_FILE_COSMAC_H
#define INCLUDE_SREC_INPUT_FILE_COSMAC_H

#pragma interface "srec_input_file_cosmac"

#include <srec/input/file.h>

/**
  * The srec_input_file_cosmac class is used to represent the parse
  * state when reading an RCS Cosmac formatted file.
  */
class srec_input_file_cosmac:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_cosmac();

    /**
      * The constructor.
      */
    srec_input_file_cosmac(const char *);

    // See base class for documentation.
    int read(srec_record &);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    /**
      * The state instance variable is used to remember where we are in
      * parsing the !M command.
      *
      *   \n  no command seen yet
      *   !   have seen the ! character
      *   M   have seen the !M characters, want an address
      *   sp  have seen !Maaaa, want data bytes
      *   ,   have seen , charcter, discarding to \n, then resume sp state
      *   ;   have seen , charcter, discarding to \n, then resume M state
      */
    char state;

    /**
      * The address instance variable is used to remember what address
      * we are up to in the input.
      */
    unsigned long address;

    /**
      * The seen_some_input instance variable is used to remember whether
      * the file has any contents or not.
      */
    bool seen_some_input;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_cosmac();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_cosmac(const srec_input_file_cosmac &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_cosmac &operator=(const srec_input_file_cosmac &);
};

#endif // INCLUDE_SREC_INPUT_FILE_COSMAC_H
