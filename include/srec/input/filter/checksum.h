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
 * MANIFEST: interface definition for lib/srec/input/filter/checksum.cc
 */

#ifndef INCLUDE_SREC_INPUT_FILTER_CHECKSUM_H
#define INCLUDE_SREC_INPUT_FILTER_CHECKSUM_H

#pragma interface "srec_input_filter_checksum"

#include <interval.h>
#include <srec/input/filter.h>
#include <srec/record.h>

class srec_input_filter_checksum: public srec_input_filter
{
public:
	virtual ~srec_input_filter_checksum();
	virtual int read(srec_record &);

protected:
	srec_input_filter_checksum(srec_input *deeper, int address, int length,
		int order, int width = 1);
	typedef unsigned long sum_t;
	int checksum_address;
	int length;
	int checksum_order;
	srec_record data;
	sum_t sum;
	int width;
	virtual sum_t calculate() = 0;

private:
	// Do not use these...
	srec_input_filter_checksum();
	srec_input_filter_checksum(const srec_input_filter_checksum &);
	srec_input_filter_checksum &operator=(const srec_input_filter_checksum &);
};

#endif /* INCLUDE_SREC_INPUT_FILTER_CHECKSUM_H */
