/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 2000, 2001 Peter Miller;
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
 * MANIFEST: interface definition for lib/srec/input/filter/checksum/negative.cc
 */

#ifndef INCLUDE_SREC_INPUT_FILTER_CHECKSUM_NEGATIVE_H
#define INCLUDE_SREC_INPUT_FILTER_CHECKSUM_NEGATIVE_H

#pragma interface "srec_input_filter_checksum_negative"

#include <srec/input/filter/checksum.h>

class srec_input_filter_checksum_negative:
	public srec_input_filter_checksum
{
public:
	virtual ~srec_input_filter_checksum_negative();
	srec_input_filter_checksum_negative(srec_input *deeper, int address,
		int length, int order, int width = 1);

protected:
	sum_t calculate();

private:
	// Do not use these...
	srec_input_filter_checksum_negative();
	srec_input_filter_checksum_negative(
		const srec_input_filter_checksum_negative &);
	srec_input_filter_checksum_negative &operator=(
		const srec_input_filter_checksum_negative &);
};

#endif /* INCLUDE_SREC_INPUT_FILTER_CHECKSUM_NEGATIVE_H */
