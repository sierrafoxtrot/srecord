//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000-2003, 2006 Peter Miller;
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
// MANIFEST: interface definition for lib/srec/output/file/ascii_hex.cc
//

#ifndef INCLUDE_SREC_OUTPUT_FILE_ASCII_HEX_H
#define INCLUDE_SREC_OUTPUT_FILE_ASCII_HEX_H


#include <srec/output/file.h>

/**
  * The srec_output_file_ascii_hex class is used to write a file in
  * Ascii-Hex-Space format.
  */
class srec_output_file_ascii_hex:
    public srec_output_file
{
public:
    /**
      * The destrutcor.
      */
    virtual ~srec_output_file_ascii_hex();

    /**
      * The constructor.
      */
    srec_output_file_ascii_hex(const char *filename);

    // See base class for documentation.
    void write(const srec_record &);

    // See base class for documentation.
    void line_length_set(int);

    // See base class for documentation.
    void address_length_set(int);

    // See base class for documentation.
    int preferred_block_size_get() const;

private:
    /**
      * The address instance variable is used to remember where we are
      * up to in the output.  Used to limit the number of $A line emitted.
      */
    unsigned long address;

    /**
      * The column instance variable is used to remember which column
      * we are up to in the output.  Used to limit the length of lines
      * in the output.
      */
    int column;

    /**
      * The pref_block_size instance variable is used to remember the
      * number of bytes in the preferred block size.
      */
    int pref_block_size;

    /**
        The fubar instance variable is used to remember
      */
    int address_length;

    /**
      * The default constructor.  Do not use.
      */
    srec_output_file_ascii_hex();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_ascii_hex(const srec_output_file_ascii_hex &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_ascii_hex &operator=(const srec_output_file_ascii_hex &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_ASCII_HEX_H
