/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998, 1999, 2000, 2001 Peter Miller;
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
 * MANIFEST: interface definition for lib/srec/input/filter.cc
 */

#ifndef INCLUDE_SREC_INPUT_FILTER_H
#define INCLUDE_SREC_INPUT_FILTER_H

#pragma interface "srec_input_filter"

#include <srec/input.h>

class srec_input_filter: public srec_input
{
protected:
	//
	// I'd like this to be private, but srec_input_filter_crc16 and
	// srec_input_filter_crc32 need to access it directly, rather than
	// going through the srec_input_filter::read method.
	//
	srec_input *ifp;

public:
	virtual ~srec_input_filter();
	srec_input_filter(srec_input *);
	virtual int read(class srec_record &);
	virtual const string filename() const;
	virtual const string filename_and_line() const;
	virtual const char *get_file_format_name() const;

private:
	// do not use these methods...
	srec_input_filter();
	srec_input_filter(const srec_input_filter &);
	srec_input_filter &operator=(const srec_input_filter &);
};

#endif /* INCLUDE_SREC_INPUT_FILTER_H */
