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
 * MANIFEST: functions to impliment the srec_input_filter_length class
 */

#pragma implementation

#include <interval.h>
#include <srec/input/filter/length.h>
#include <srec/record.h>


srec_input_filter_length::srec_input_filter_length()
	: srec_input_filter(), length_address(0), length_length(0),
		length_order(0), range(), data()
{
}


srec_input_filter_length::srec_input_filter_length(srec_input *a1, int a2,
		int a3, int a4)
	: srec_input_filter(a1), length_address(a2), length_length(a3),
		length_order(a4), range(), data()
{
	if (length_length < 0)
		length_length = 0;
	else if (length_length > 8)
		length_length = 8;
	range += interval(length_address, length_address + length_length);
}


srec_input_filter_length::srec_input_filter_length(
		const srec_input_filter_length &arg)
	: srec_input_filter(arg), length_address(arg.length_address),
		length_length(arg.length_length),
		length_order(arg.length_order), range(), data()
{
	if (length_length < 0)
		length_length = 0;
	else if (length_length > 8)
		length_length = 8;
	range += interval(length_address, length_address + length_length);
}


srec_input_filter_length &
srec_input_filter_length::operator=(const srec_input_filter_length &arg)
{
	srec_input_filter::operator=(arg);
	length_address = arg.length_address;
	length_length = arg.length_length;
	length_order = arg.length_order;

	if (length_length < 0)
		length_length = 0;
	else if (length_length > 8)
		length_length = 8;
	range = interval(length_address, length_address + length_length);
	return *this;
}


srec_input_filter_length::~srec_input_filter_length()
{
}


int
srec_input_filter_length::read(srec_record &record)
{
	if (data.get_type() == srec_record::type_unknown)
	{
		if (!srec_input_filter::read(data))
		{
			if (length_length > 0)
				goto generate;
			return 0;
		}
	}
	switch (data.get_type())
	{
	default:
		break;

	case srec_record::type_data:
		range +=
			interval
			(
				data.get_address(),
				data.get_address() + data.get_length()
			);
		break;

	case srec_record::type_termination:
		if (length_length <= 0)
			break;
		generate:
		if (length_length > 8)
			length_length = 8;
		unsigned char chunk[8];
		int length = range.get_highest() - range.get_lowest();
		if (length_order)
			srec_record::encode_little_endian(chunk, length, length_length);
		else
			srec_record::encode_big_endian(chunk, length, length_length);
		record =
			srec_record
			(
				srec_record::type_data,
				length_address,
				chunk,
				length_length
			);
		length_length = 0;
		return 1;
	}
	record = data;
	data = srec_record();
	return 1;
}
