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
 * MANIFEST: interface definition for include/srec/input/file/signetics.cc
 */

#ifndef INCLUDE_SREC_INPUT_FILE_SIGNETICS_H
#define INCLUDE_SREC_INPUT_FILE_SIGNETICS_H

#pragma interface "srec_input_file_signetics"

#include <srec/input/file.h>

/**
  * The srec_input_file_signetics class is used to represent reading a
  * Signetics format file.
  */
class srec_input_file_signetics:
	public srec_input_file
{
public:
	/**
	  * The default constructor.  The input will be read from the
	  * standard input.
	  */
	srec_input_file_signetics();

	/**
	  * The constructor.  The input will be read from the named file
	  * (or the standatd input if the filename is "-").
	  */
	srec_input_file_signetics(const char *);

	/**
	  * The destructor.
	  */
	virtual ~srec_input_file_signetics();

	// See base class for documentation.
	int read(srec_record &);

	// See base class for documentation.
	const char *get_file_format_name() const;

protected:
	/**
	  * See base class for documentation.  We over-ride this method
	  * because signetics uses its own XOR-ROL checksum algorithm.
	  */
	void checksum_add(unsigned char);

private:
	/**
	  * The copy constructor.  Do not use.
	  */
	srec_input_file_signetics(const srec_input_file_signetics &);

	/**
	  * The assignment operator.  Do not use.
	  */
	srec_input_file_signetics &operator=(const srec_input_file_signetics &);

	/**
	  * The read_inner method is used to read one line/record from
	  * the input.  Used by the `read' method.
	  */
	int read_inner(srec_record &);

	/**
	  * The garbage_warning instance variable is used to remember
	  * whther or not a warning has been issued about non-format
	  * lines in the file.  Only one warning per file is issued.
	  */
	bool garbage_warning;

	/**
	  * The seen_some_input instance variable is used to
	  * remember whether any input has been seen.
	  */
	bool seen_some_input;
};

#endif /* INCLUDE_SREC_INPUT_FILE_SIGNETICS_H */
