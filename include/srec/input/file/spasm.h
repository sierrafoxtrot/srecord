//
//	srecord - manipulate eprom load files
//	Copyright (C) 2001-2003 Peter Miller;
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
// MANIFEST: interface definition for include/srec/input/file/spasm.cc
//

#ifndef INCLUDE_SREC_INPUT_FILE_SPASM_H
#define INCLUDE_SREC_INPUT_FILE_SPASM_H

#pragma interface "srec_input_file_spasm"

#include <srec/input/file.h>

/**
  * The srec_input_file_spasm class is used to represent the parse state
  * of a SPASM formatted input file.
  */
class srec_input_file_spasm:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_spasm();

    /**
      * The constructor.
      */
    srec_input_file_spasm(const char *filename, bool bigendian = true);

    // See base class for documentation.
    int read(srec_record &);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    /**
      * The read_inner method is used to read a record from the file.
      * The read method is a wrapper around it.
      */
    int read_inner(srec_record &);

    /**
      * The seen_some_input instance variable is used to remember whether
      * or not any data has been seen from this file to date.
      */
    bool seen_some_input;

    /**
      * The bigend instance variable is used to remember whether the
      * file is big-endian (true) or little-endian (false).
      */
    bool bigend;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_spasm();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_spasm(const srec_input_file_spasm &);

    /**
      * The assigmne toperator.  Do not use.
      */
    srec_input_file_spasm &operator=(const srec_input_file_spasm &);
};

#endif // INCLUDE_SREC_INPUT_FILE_SPASM_H
