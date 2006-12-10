//
//	srecord - manipulate eprom load files
//	Copyright (C) 2003, 2005, 2006 Peter Miller;
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
// MANIFEST: interface definition for include/srec/output/file/needham.cc
//

#ifndef INCLUDE_SREC_OUTPUT_FILE_NEEDHAM_H
#define INCLUDE_SREC_OUTPUT_FILE_NEEDHAM_H


#include <srec/output/file.h>

/**
  * The srec_output_file_needham class is used to represent
  */
class srec_output_file_needham:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_needham();

    /**
      * The constructor.
      */
    srec_output_file_needham(const char *);

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
      * The address instance variable is used to remember where in the
      * file the output has reached.  This is used to fill gaps.
      */
    unsigned long address;

    /**
      * The column instance variable is used to remember the column of
      * the output text we have reached.  This is used when calculating
      * when to throw new lines.
      */
    int column;

    /**
      * The pref_block_size instance variable is used to remember The
      * preferred line length of the output text.
      */
    int pref_block_size;

    /**
      * The address_length instance variable is used to remember the
      * minimum number of bytes to use for addresses in the output text.
      */
    int address_length;

    /**
      * The default constructor.  Do not use.
      */
    srec_output_file_needham();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_needham(const srec_output_file_needham &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_needham &operator=(const srec_output_file_needham &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_NEEDHAM_H
