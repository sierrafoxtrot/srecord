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
// MANIFEST: interface definition for include/srec/input/file/fairchild.cc
//

#ifndef INCLUDE_SREC_INPUT_FILE_FAIRCHILD_H
#define INCLUDE_SREC_INPUT_FILE_FAIRCHILD_H

#pragma interface "srec_input_file_fairchild"

#include <srec/input/file.h>

/**
  * The srec_input_file_fairchild class is used to represent the parse
  * stat ewhen reading a file in Fairchild Fairbug format.
  */
class srec_input_file_fairchild:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_fairchild();

    /**
      * The constructor.
      */
    srec_input_file_fairchild(const char *filename);

    // See base class for documentation.
    int read(srec_record &);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    /**
      * The get_nibble method gets a single hex-digit from input.
      * We override the one in the base class because the checksum is
      * nibble-based, not byte-based.
      */
    int get_nibble();

    /**
      * The get_byte method gets a two hex-digit from input and assembles
      * them (big-endian) into a byte.  We override the one in the base
      * class because the checksum is nibble-based, not byte-based.
      */
    int get_byte();

    /**
      * The header_seen instance variable is used to member whether the
      * file header has been processed yet.
      */
    bool header_seen;

    /**
      * The address instance variable is used to member the address at
      * the currebt point in the input file.
      */
    unsigned long address;

    /**
      * The file_contains_data instance variable is used to member
      * whether any data records have been seen yet.
      */
    bool file_contains_data;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_fairchild();

    /**
      * T  Do not use.he copy constructor.  Do not use.
      */
    srec_input_file_fairchild(const srec_input_file_fairchild &);

    /**
      * The assignment operator.
      */
    srec_input_file_fairchild &operator=(const srec_input_file_fairchild &);
};

#endif // INCLUDE_SREC_INPUT_FILE_FAIRCHILD_H
