//
//	srecord - manipulate eprom load files
//	Copyright (C) 2001-2003, 2006 Peter Miller;
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
// MANIFEST: interface definition for include/srec/input/file/atmel_generic.cc
//

#ifndef INCLUDE_SREC_INPUT_FILE_ATMEL_GENERIC_H
#define INCLUDE_SREC_INPUT_FILE_ATMEL_GENERIC_H


#include <srec/input/file.h>

/**
  * The srec_input_file_atmel_generic class is used to represent the
  * parse stat of an Atmel Genric formatted file.
  */
class srec_input_file_atmel_generic:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_atmel_generic();

    /**
      * the constructor.
      */
    srec_input_file_atmel_generic(const char *, bool bigendian = true);

    // See base class for documentation.
    int read(srec_record &);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    /**
      * The read_inner method is used to read a single record form
      * the file.  The read method is a wrapper around it.
      */
    int read_inner(srec_record &);

    /**
      * The seen_some_input instance variable is used to remember whther
      * any input data has been seen to date.
      */
    bool seen_some_input;

    /**
      * The bigend instance variable is used to remember whether the
      * input is bigendian (true) or little endian (false).
      */
    bool bigend;

    /**
      * The deafult constructor.  Do not use.
      */
    srec_input_file_atmel_generic();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_atmel_generic(const srec_input_file_atmel_generic &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_atmel_generic &operator=(
	const srec_input_file_atmel_generic &);
};

#endif // INCLUDE_SREC_INPUT_FILE_ATMEL_GENERIC_H
