//
//	srecord - manipulate eprom load files
//	Copyright (C) 2001, 2003, 2006 Peter Miller;
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
// MANIFEST: interface definition for include/srec/input/file/fastload.cc
//

#ifndef INCLUDE_SREC_INPUT_FILE_FASTLOAD_H
#define INCLUDE_SREC_INPUT_FILE_FASTLOAD_H


#include <lib/srec/input/file.h>

/**
  * The fastload class is used to parse an LSI Logic Fast Load format file.
  */
class srec_input_file_fastload:
    public srec_input_file
{
public:
    /**
      * A constructor.  The input is read from the named file (or
      * the standard input if the file anme is "-").
      */
    srec_input_file_fastload(const char *filename);

    /**
      * The destructor.
      */
    virtual ~srec_input_file_fastload();

    // See base class for documentation.
    int read(srec_record &);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    /**
      * Read one line (record) of input.  Called by the read() method.
      * Returns false at end of file.
      */
    int read_inner(srec_record &);

    /**
      * Get a singe base-64 digit.  Returns 0..63 for a valid digit.
      * Fatal error if not a valid base-64 digit.
      */
    int get_digit(void);

    /**
      * The get_number method is used to get a base-64 number.
      * The ordering is big-endian, but like ordinary decimal numbers.
      * Four digits is 24 bits.
      */
    unsigned long get_number(int min_digits, int max_digits);

    /**
      * The seen_some_input instance variable is used to
      * remember whether any input has been seen.
      */
    bool seen_some_input;

    /**
      * The address instance variable is used to represent the
      * current address within the file.  It is set by the /A command,
      * and advanced by the data, /B and /Z commands.
      */
    unsigned long address;

    /**
      * The expect_white_space method is used to ensure that while
      * space follows the various commands.  It doesn't consume any
      * input, since it uses peek_char to do the checking.
      */
    void expect_white_space();

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_fastload();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_fastload(const srec_input_file_fastload &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_fastload &operator=(const srec_input_file_fastload &);
};

#endif // INCLUDE_SREC_INPUT_FILE_FASTLOAD_H
