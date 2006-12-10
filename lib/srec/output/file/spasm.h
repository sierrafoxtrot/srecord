//
//	srecord - manipulate eprom load files
//	Copyright (C) 2001, 2002, 2005, 2006 Peter Miller;
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
// MANIFEST: interface definition for include/srec/output/file/spasm.cc
//

#ifndef INCLUDE_SREC_OUTPUT_FILE_SPASM_H
#define INCLUDE_SREC_OUTPUT_FILE_SPASM_H


#include <lib/srec/output/file.h>

/**
  * The srec_output_file_spasm class is used to represent an output
  * files in SPASM format.
  */
class srec_output_file_spasm:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_spasm();

    /**
      * The default constructor.
      * Output will be written to the standard output.
      */
    srec_output_file_spasm();

    /**
      * The constructor.
      *
      * @param filename
      *     The name of the file to write the output to.  The string "-"
      *     is understood to mean the standrad output.
      * @param bigendian
      *     Whether to output big endian data (true) or little endian
      *     data (false).
      */
    srec_output_file_spasm(const char *filename, bool bigendian = true);

    // See base class for documentation.
    virtual void write(const srec_record &);

    // See base class for documentation.
    virtual void line_length_set(int);

    // See base class for documentation.
    virtual void address_length_set(int);

    // See base class for documentation.
    virtual int preferred_block_size_get() const;

private:
    /**
      * The bigend instance variable is used to remember whether to
      * output big endian data (true) or little endian data (false).
      */
    bool bigend;

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_spasm(const srec_output_file_spasm &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_spasm &operator=(const srec_output_file_spasm &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_SPASM_H
