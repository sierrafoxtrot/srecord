//
//	srecord - manipulate eprom load files
//	Copyright (C) 2001, 2002, 2005 Peter Miller;
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
// MANIFEST: interface definition for include/srec/output/file/signetics.cc
//

#ifndef INCLUDE_SREC_OUTPUT_FILE_SIGNETICS_H
#define INCLUDE_SREC_OUTPUT_FILE_SIGNETICS_H

#pragma interface "srec_output_file_signetics"

#include <srec/output/file.h>

class srec_record; // forward

/**
  * The srec_output_file_signetics class is used to represent output to
  * a file using the Signetiocs format.
  */
class srec_output_file_signetics:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_signetics();

    /**
      * The default constructor.  The output will be sent to the
      * standard output.
      */
    srec_output_file_signetics();

    /**
      * A constructor.  The output will be sent to the named file (or
      * the standard output if the file nameis "-").
      */
    srec_output_file_signetics(const char *);

    // See base class for documentation.
    virtual void write(const srec_record &);

    // See base class for documentation.
    virtual void line_length_set(int);

    // See base class for documentation.
    virtual void address_length_set(int);

    // See base class for documentation.
    virtual int preferred_block_size_get() const;

    /**
      * See base class for documentation.  We over-ride the base
      * implementation because signetics uses its own XOR-ROL algorithm.
      */
    void checksum_add(unsigned char);

private:
    /**
      * The pref_block_size is used to remember the preferred block
      * size.  Set by the line_length_set() method.  Read by the
      * preferred_block_size_get() method.
      */
    int pref_block_size;

    /**
      * The write_inner method is used to write one line/record to the
      * output.  It is called by the write() method.
      */
    void write_inner(int, unsigned long, int, const void *, int);

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_signetics(const srec_output_file_signetics &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_signetics &operator=(const srec_output_file_signetics &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_SIGNETICS_H
