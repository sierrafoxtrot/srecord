/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998, 1999 Peter Miller;
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
 * MANIFEST: functions to impliment the srec_input_filter_checksum class
 */

#pragma implementation "srec_input_filter_checksum"

#include <interval.h>
#include <srec/input/filter/checksum.h>
#include <srec/record.h>


srec_input_filter_checksum::srec_input_filter_checksum()
	: srec_input_filter(), checksum_address(0), length(0),
		checksum_order(0), data(), sum(0), width(1)
{
}


srec_input_filter_checksum::srec_input_filter_checksum(srec_input *a1, int a2,
		int a3, int a4, int a5)
	: srec_input_filter(a1), checksum_address(a2), length(a3),
		checksum_order(!!a4), data(), sum(0), width(a5)
{
	if (length < 0)
		length = 0;
	else if (length > (int)sizeof(sum_t))
		length = sizeof(sum_t);
	if (width < 1)
		width = 1;
	else if (width > length)
	    	width = length;
}


srec_input_filter_checksum::srec_input_filter_checksum(
		const srec_input_filter_checksum &arg)
	: srec_input_filter(arg), checksum_address(arg.checksum_address),
		length(arg.length), checksum_order(arg.checksum_order), data(),
		sum(0), width(arg.width)
{
	if (length < 0)
		length = 0;
	else if (length > (int)sizeof(sum_t))
		length = sizeof(sum_t);
	if (width < 1)
		width = 1;
	else if (width > length)
	    	width = length;
}


srec_input_filter_checksum &
srec_input_filter_checksum::operator=(const srec_input_filter_checksum &arg)
{
	if (this != &arg)
	{
		srec_input_filter::operator=(arg);
		checksum_address = arg.checksum_address;
		length = arg.length;
		checksum_order = arg.checksum_order;
		sum = 0;
		width = arg.width;

		if (length < 0)
			length = 0;
		else if (length > (int)sizeof(sum_t))
			length = sizeof(sum_t);
		if (width < 1)
			width = 1;
		else if (width > length)
	    		width = length;
	}
	return *this;
}


srec_input_filter_checksum::~srec_input_filter_checksum()
{
}


int
srec_input_filter_checksum::read(srec_record &record)
{
	if (data.get_type() == srec_record::type_unknown)
	{
		if (!srec_input_filter::read(data))
		{
			if (length > 0)
				goto generate;
			return 0;
		}
	}
	switch (data.get_type())
	{
	default:
		break;

	case srec_record::type_data:
		if (width <= 1)
		{
			for (int j = 0; j < data.get_length(); ++j)
			{
				sum += data.get_data(j);
			}
		}
		else if (checksum_order)
		{
			// Little endian
			for (int j = 0; j < data.get_length(); ++j)
			{
				sum += (sum_t)data.get_data(j) << (8 *
					((data.get_address() + j) % width));
			}
		}
		else
		{
			// Big endian
			for (int j = 0; j < data.get_length(); ++j)
			{
				sum += (sum_t)data.get_data(j) << (8 *
					(width - 1 - ((data.get_address() + j)
					% width)));
			}
		}
		break;

	case srec_record::type_termination:
		if (length <= 0)
			break;
		generate:
		unsigned char chunk[sizeof(sum_t)];
		sum_t value = calculate();
		if (checksum_order)
			srec_record::encode_little_endian(chunk, value, length);
		else
			srec_record::encode_big_endian(chunk, value, length);
		record =
			srec_record
			(
				srec_record::type_data,
				checksum_address,
				chunk,
				length
			);
		length = 0;
		return 1;
	}
	record = data;
	data = srec_record();
	return 1;
}
