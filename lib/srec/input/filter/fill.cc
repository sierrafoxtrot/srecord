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
 * MANIFEST: functions to impliment the srec_input_filter_fill class
 */

#pragma implementation "srec_input_filter_fill"

#include <interval.h>
#include <srec/input/filter/fill.h>
#include <srec/record.h>


srec_input_filter_fill::srec_input_filter_fill()
	: srec_input_filter(), filler_value(0), filler_block(0), range(), data()
{
}


srec_input_filter_fill::srec_input_filter_fill(srec_input *a1, int a2,
		const interval &a3)
	: srec_input_filter(a1), filler_value(a2), filler_block(0), range(a3),
		data()
{
}


srec_input_filter_fill::srec_input_filter_fill(const srec_input_filter_fill &arg)
	: srec_input_filter(arg), filler_value(arg.filler_value),
		filler_block(0), range(arg.range), data()
{
}


srec_input_filter_fill &
srec_input_filter_fill::operator=(const srec_input_filter_fill &arg)
{
	srec_input_filter::operator=(arg);
	filler_value = arg.filler_value;
	if (filler_block)
	{
		delete filler_block;
		filler_block = 0;
	}
	range = arg.range;
	data = srec_record();
	return *this;
}


srec_input_filter_fill::~srec_input_filter_fill()
{
	if (filler_block)
		delete filler_block;
}


int
srec_input_filter_fill::read(srec_record &record)
{
	if (data.get_type() == srec_record::type_unknown)
	{
		if (!srec_input_filter::read(data))
		{
			if (!range.empty())
				goto generate;
			return 0;
		}
	}
	switch (data.get_type())
	{
	default:
		break;

	case srec_record::type_data:
		range -=
			interval
			(
				data.get_address(),
				data.get_address() + data.get_length()
			);
		break;

	case srec_record::type_termination:
		if (range.empty())
			break;
		generate:
		interval chunk(range.get_lowest(), range.get_lowest() + 256);
		chunk *= range;
		chunk.first_interval_only();
		if (!filler_block)
		{
			filler_block = new unsigned char [256];
			memset(filler_block, filler_value, 256);
		}
		record =
			srec_record
			(
				srec_record::type_data,
				chunk.get_lowest(),
				filler_block,
				chunk.get_highest() - chunk.get_lowest()
			);
		range -= chunk;
		return 1;
	}
	record = data;
	data = srec_record();
	return 1;
}
