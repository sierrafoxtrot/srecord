/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 2000 Peter Miller;
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
 * MANIFEST: interface definition for include/srec/input/file/mos_tech.cc
 */

#ifndef INCLUDE_SREC_INPUT_FILE_MOS_TECH_H
#define INCLUDE_SREC_INPUT_FILE_MOS_TECH_H

#pragma interface "srec_input_file_mos_tech"

#include <srec/input/file.h>

/**
  * The mos_tech class is used to parse a Mos Technologies format file.
  */
class srec_input_file_mos_tech: public srec_input_file
{
public:
	/**
	  * The default constructor.  The input is read from the
	  * standard input.
	  */
	srec_input_file_mos_tech();

	/**
	  * A constructor.  The input is read from the named file (or
	  * the standard input if the file anme is "-").
	  */
	srec_input_file_mos_tech(const char *filename);

	/**
	  * The destructor.
	  */
	virtual ~srec_input_file_mos_tech();

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

	/**
	  * The copy constructor.  Do not use.
	  */
	srec_input_file_mos_tech(const srec_input_file_mos_tech &);

	/**
	  * The assignment operator.  Do not use.
	  */
	srec_input_file_mos_tech &operator=(const srec_input_file_mos_tech &);
};

#endif /* INCLUDE_SREC_INPUT_FILE_MOS_TECH_H */
