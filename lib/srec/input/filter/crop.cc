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
 * MANIFEST: functions to impliment the srec_input_filter_crop class
 */

#pragma implementation

#include <srec/input/filter/crop.h>
#include <srec/record.h>


srec_input_filter_crop::srec_input_filter_crop()
	: srec_input_filter(), lo(0), hi(0)
{
}


srec_input_filter_crop::srec_input_filter_crop(srec_input *a1,
		unsigned long a2, unsigned long a3)
	: srec_input_filter(a1), lo(a2), hi(a3)
{
}


srec_input_filter_crop::srec_input_filter_crop(const srec_input_filter_crop &arg)
	: srec_input_filter(arg), lo(arg.lo), hi(arg.hi)
{
}


srec_input_filter_crop &
srec_input_filter_crop::operator=(const srec_input_filter_crop &arg)
{
	srec_input_filter::operator=(arg);
	lo = arg.lo;
	hi = arg.hi;
	return *this;
}


srec_input_filter_crop::~srec_input_filter_crop()
{
}


int
srec_input_filter_crop::read(srec_record &record)
{
	for (;;)
	{
		if (!srec_input_filter::read(record))
			return 0;
		switch (record.get_type())
		{
		default:
			break;

		case srec_record::type_data:
			{
				unsigned long address = record.get_address();
				int length = record.get_length();
				const unsigned char *data = record.get_data();
				if (address >= lo)
				{
					if (hi && address >= hi)
						continue;
					if (hi && address + length > hi)
						record.set_length(hi - address);
					break;
				}
				if (address + length <= lo)
					continue;
				int offset = lo - address;
				address += offset;
				data += offset;
				length -= offset;
				if (hi && address + length > hi)
					length = hi - address;
				record =
					srec_record
					(
						srec_record::type_data,
						address,
						data,
						length
					);
			}
			break;

		case srec_record::type_termination:
			if
			(
				record.get_address() < lo
			||
				(hi && record.get_address() >= hi)
			)
				continue;
			break;
		}
		return 1;
	}
}
