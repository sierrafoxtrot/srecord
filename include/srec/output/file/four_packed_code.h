/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 2001 Peter Miller;
 *	All rights reserved.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 *
 * MANIFEST: interface definition for include/srec/output/file/four_packed_code.cc
 */

#ifndef INCLUDE_SREC_OUTPUT_FILE_FOUR_PACKED_CODE_H
#define INCLUDE_SREC_OUTPUT_FILE_FOUR_PACKED_CODE_H

#pragma interface "srec_output_file_four_packed_code"

#include <srec/output/file.h>

/**
  * The srec_output_file_four_packed_code class is used to emit files
  * in the Four Packed Code (FPC) format.
  */
class srec_output_file_four_packed_code:
	public srec_output_file
{
public:
	/**
	  * The destructor.
	  */
	virtual ~srec_output_file_four_packed_code();

	/**
	  * The default constructor.  Output will be sent to the standard
	  * output.
	  */
	srec_output_file_four_packed_code();

	/**
	  * The constructor.  Output will be sent to the named file
	  * (or the standard output if the file name is "-").
	  */
	srec_output_file_four_packed_code(const char *);

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
	  * The pref_block_size instance variable is used to
	  * remember the preferred block size.	Read by the the
	  * preferred_block_size_get method.  Set by the line_length_set
	  * method.
	  */
	int pref_block_size;

	/**
	  * The put_byte_pos instance variable is used by the put_byte
	  * method to remember where wea re positioned within each
	  * 4-byte chunk.
	  */
	unsigned put_byte_pos;

	/**
	  * The put_byte_value instance variable is used by the put_byte
	  * method to remember the cumulative value of each 4-byte chunk.
	  * At the end of 4 bytes, the 5-character base85 encoding
	  * is issued.
	  */
	unsigned long put_byte_value;

	/**
	  * The write_inner method is used by the write method to
	  * write a single line of output.
	  */
	void write_inner(unsigned long address, const void *data,
		int data_length);

	/**
	  * See base class for documentation.  We over-ride this method
	  * so that we can do the base85 encoding of each 4-byte chunk.
	  */
	void put_byte(unsigned char);

	/**
	  * The copy constructor.  Do not use.
	  */
	srec_output_file_four_packed_code(
		const srec_output_file_four_packed_code &);

	/**
	  * The assignment operator.  Do not use.
	  */
	srec_output_file_four_packed_code &operator=(
		const srec_output_file_four_packed_code &);
};

#endif /* INCLUDE_SREC_OUTPUT_FILE_FOUR_PACKED_CODE_H */
