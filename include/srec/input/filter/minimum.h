/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998, 1999, 2001 Peter Miller;
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
 * MANIFEST: interface definition for lib/srec/input/filter/minimum.cc
 */

#ifndef INCLUDE_SREC_INPUT_FILTER_MINIMUM_H
#define INCLUDE_SREC_INPUT_FILTER_MINIMUM_H

#pragma interface "srec_input_filter_minimum"

#include <srec/input/filter.h>

class srec_input_filter_minimum: public srec_input_filter
{
public:
	virtual ~srec_input_filter_minimum();
	srec_input_filter_minimum(srec_input *, int, int, int);
	virtual int read(srec_record &);

private:
	int minimum_address;
	int minimum_length;
	int minimum_order;
	unsigned long minimum;
	bool minimum_set;
	srec_record *data;

	// Do not use these...
	srec_input_filter_minimum();
	srec_input_filter_minimum(const srec_input_filter_minimum &);
	srec_input_filter_minimum &operator=(const srec_input_filter_minimum &);
};

#endif /* INCLUDE_SREC_INPUT_FILTER_MINIMUM_H */
