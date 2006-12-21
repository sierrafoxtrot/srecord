//
//	srecord - manipulate eprom load files
//	Copyright (C) 2003, 2006 Peter Miller
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
// MANIFEST: interface definition for include/srec/input/file/needham.cc
//

#ifndef INCLUDE_SREC_INPUT_FILE_NEEDHAM_H
#define INCLUDE_SREC_INPUT_FILE_NEEDHAM_H


#include <lib/srec/input/file.h>

/**
  * The srec_input_file_needham class is used to represent the parse
  * state of a file in Needham Hex format.
  */
class srec_input_file_needham:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_needham();

    /**
      * The constructor.
      */
    srec_input_file_needham(const char *);

    // See base class for documentation.
    int read(srec_record &);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    int read_inner(srec_record &);
    bool garbage_warning;
    bool seen_some_input;
    unsigned long address;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_needham();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_needham(const srec_input_file_needham &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_needham &operator=(const srec_input_file_needham &);
};

#endif // INCLUDE_SREC_INPUT_FILE_NEEDHAM_H
