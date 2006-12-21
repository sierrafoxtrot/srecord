//
//	srecord - manipulate eprom load files
//	Copyright (C) 2002, 2006 Peter Miller
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
// MANIFEST: interface definition for include/srec/input/file/os65v.cc
//

#ifndef INCLUDE_SREC_INPUT_FILE_OS65V_H
#define INCLUDE_SREC_INPUT_FILE_OS65V_H


#include <lib/srec/input/file.h>

/**
  * The srec_input_file_os65v class is used to represent the input state
  * of an Ohio Scientific hexadecimal formatted file.
  */
class srec_input_file_os65v:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_os65v();

    /**
      * The constructor.
      */
    srec_input_file_os65v(const char *);

    // See base class for documentation.
    int read(srec_record &);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_os65v();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_os65v(const srec_input_file_os65v &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_os65v &operator=(const srec_input_file_os65v &);

    /**
      * The read_inner method is used by the read method to get
      * another portion of input.
      */
    int read_inner(srec_record &);

    /**
      * The seen_some_input instance variable is used to remember whether
      * or not some valid inpout has been seen.
      */
    bool seen_some_input;

    /**
      * The address instance variable is used to remember the current
      * address.  It is advanced for every byte read.
      */
    unsigned long address;

    /**
      * The state instance variable is used to member the current input
      * state (unknown, address, data).
      */
    char state;

    /**
      * The ignore_the_rest instance variable is used to remember when
      * it is time to ignore the rest of the file.  This happens after a
      * "return to monitor" sequence, or after a "GO" command.
      */
    bool ignore_the_rest;
};

#endif // INCLUDE_SREC_INPUT_FILE_OS65V_H
