/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998 Peter Miller;
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
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * MANIFEST: interface definition for lib/srec/input/filter/split.cc
 */

#ifndef INCLUDE_SREC_INPUT_FILTER_SPLIT_H
#define INCLUDE_SREC_INPUT_FILTER_SPLIT_H

#pragma interface "srec_input_filter_split"

#include <srec/input/filter.h>
#include <srec/record.h>

class srec_input_filter_split: public srec_input_filter
{
public:
	virtual ~srec_input_filter_split();
	srec_input_filter_split(srec_input *, int, int, int);
	virtual int read(srec_record &);

private:
	int modulus;
	int offset;
	int width;
	srec_record buffer;
	int buffer_pos;

	srec_input_filter_split();
	srec_input_filter_split(const srec_input_filter_split &);
	srec_input_filter_split &operator=(const srec_input_filter_split &);
};

#endif /* INCLUDE_SREC_INPUT_FILTER_SPLIT_H */
