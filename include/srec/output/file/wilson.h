//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000-2002, 2005 Peter Miller;
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
// MANIFEST: interface definition for lib/srec/output/file/wilson.cc
//

#ifndef INCLUDE_SREC_OUTPUT_FILE_WILSON_H
#define INCLUDE_SREC_OUTPUT_FILE_WILSON_H

#pragma interface "srec_output_file_wilson"

#include <srec/output/file.h>

/**
  * The srec_output_file_wilson class is used to
  * represent an output file which is in Tektronix format.
  */
class srec_output_file_wilson:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_wilson();

    /**
      * The default constructor.
      * Output will be written to the standard output.
      */
    srec_output_file_wilson();

    /**
      * The constructor.
      *
      * @param filename
      *     The file name to open to write data to.  The file name "-"
      *     is understood to mean the standard output.
      */
    srec_output_file_wilson(const char *);

    // See base class for documentation.
    void write(const srec_record &);

    // See base class for documentation.
    virtual void line_length_set(int);

    // See base class for documentation.
    virtual void address_length_set(int);

    // See base class for documentation.
    virtual int preferred_block_size_get() const;

    // See base class for documentation.
    const char *mode() const;

protected:
    // See base class for documentation.
    virtual void put_byte(unsigned char);

private:
    /**
      * The pref_block_size instance variable is used to remember the
      * preferred number of data bytes (NOT encoded hex characters) to
      * be placed in each output line.
      */
    int pref_block_size;

    /**
      * The write_inner method is used to write a line of output.
      *
      * @param tag
      *     The tag value at the start of the line.
      * @param address
      *     The address of the first byte of data on the line.
      * @param data
      *     The palyload of this line.
      * @param data_nbytes
      *     The number of bytes of payload.
      */
    void write_inner(int tag, unsigned long address, const void *data,
	int data_nbytes);

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_wilson(const srec_output_file_wilson &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_wilson &operator=(const srec_output_file_wilson &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_WILSON_H
