//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000-2002 Peter Miller;
//	All rights reserved.
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
// MANIFEST: functions to impliment the srec_input_filter_byte_swap class
//

#pragma implementation "srec_input_filter_byte_swap"

#include <srec/input/filter/byte_swap.h>
#include <srec/record.h>


srec_input_filter_byte_swap::srec_input_filter_byte_swap(srec_input *arg) :
	srec_input_filter(arg),
	buffer(),
	buffer_pos(0)
{
}


srec_input_filter_byte_swap::~srec_input_filter_byte_swap()
{
}


int
srec_input_filter_byte_swap::read(srec_record &record)
{
	for (;;)
	{
		while
		(
			buffer.get_type() != srec_record::type_data
		||
			buffer_pos >= buffer.get_length()
		)
		{
			if (!srec_input_filter::read(buffer))
				return 0;
			if (buffer.get_type() != srec_record::type_data)
			{
				record = buffer;
				record.set_address(record.get_address() ^ 1);
				return 1;
			}
			buffer_pos = 0;
		}

		unsigned long addr = ((buffer.get_address() + buffer_pos) ^ 1);
		unsigned char c = buffer.get_data(buffer_pos++);
		record = srec_record(srec_record::type_data, addr, &c, 1);
		return 1;
	}
}
