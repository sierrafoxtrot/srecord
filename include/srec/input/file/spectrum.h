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
// MANIFEST: interface definition for include/srec/input/file/spectrum.cc
//

#ifndef INCLUDE_SREC_INPUT_FILE_SPECTRUM_H
#define INCLUDE_SREC_INPUT_FILE_SPECTRUM_H

#pragma interface "srec_input_file_spectrum"

#include <srec/input/file.h>

/**
  * The srec_input_file_spectrum class is used to represent the parse
  * state when reading from a file in Spectrum format.
  */
class srec_input_file_spectrum:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_spectrum();

    /**
      * The constructor.
      */
    srec_input_file_spectrum(const char *filename);

    // See base class for documentation.
    int read(srec_record &);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    /**
      * The header_seen instance variable is used to remember whether
      * the file header (the start character STX=0x02) has been seen yet.
      */
    bool header_seen;

    /**
      * The trailer_seen instance variable is used to remember whether
      * the file trailer (the end character ETX=0x03) has been seen yet.
      */
    bool trailer_seen;

    /**
      * The file_contains_data instance variable is used to remember
      * whether any file data has been seen yet.
      */
    bool file_contains_data;

    /**
      * The get_decimal method is used to get a decimal number from
      * the input.  It must have at least one digit.
      */
    int get_decimal();

    /**
      * The get_binary method is used to get a binary number from
      * the input.  It must have at least one digit.
      */
    int get_binary();

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_spectrum();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_spectrum(const srec_input_file_spectrum &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_spectrum &operator=(const srec_input_file_spectrum &);
};

#endif // INCLUDE_SREC_INPUT_FILE_SPECTRUM_H
