//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998, 1999, 2001, 2002, 2006 Peter Miller
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: functions to impliment the srec_input_filter_maximum class
//


#include <lib/interval.h>
#include <lib/srec/input/filter/maximum.h>
#include <lib/srec/record.h>


srec_input_filter_maximum::srec_input_filter_maximum(srec_input *a1, int a2,
		int a3, int a4) :
	srec_input_filter(a1),
	maximum_address(a2),
	maximum_length(a3),
	maximum_order(a4),
	maximum(0),
	maximum_set(false)
{
	if (maximum_length < 0)
		maximum_length = 0;
	else if (maximum_length > 8)
		maximum_length = 8;
	maximum = maximum_address + maximum_length;
	maximum_set = true;
}


srec_input_filter_maximum::~srec_input_filter_maximum()
{
}


int
srec_input_filter_maximum::generate(srec_record &record)
{
	if (maximum_length <= 0)
		return 0;
	if (maximum_length > 8)
		maximum_length = 8;
	unsigned char chunk[8];
	if (maximum_order)
		srec_record::encode_little_endian(chunk, maximum, maximum_length);
	else
		srec_record::encode_big_endian(chunk, maximum, maximum_length);
	record =
		srec_record
		(
			srec_record::type_data,
			maximum_address,
			chunk,
			maximum_length
		);
	maximum_length = 0;
	return 1;
}


int
srec_input_filter_maximum::read(srec_record &record)
{
	if (!srec_input_filter::read(record))
		return generate(record);
	if (record.get_type() == srec_record::type_data)
	{
		if (!maximum_set || maximum < record.get_address_end())
		{
			maximum = record.get_address_end();
			maximum_set = true;
		}
	}
	return 1;
}
