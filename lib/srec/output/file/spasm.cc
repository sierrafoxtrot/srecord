//
//	srecord - manipulate eprom load files
//	Copyright (C) 2001, 2002, 2006 Peter Miller;
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
// MANIFEST: functions to impliment the srec_output_file_spasm class
//


#include <lib/srec/output/file/spasm.h>
#include <lib/srec/record.h>


srec_output_file_spasm::srec_output_file_spasm(const char *filename,
		bool endianness) :
	srec_output_file(filename),
	bigend(endianness)
{
}


srec_output_file_spasm::~srec_output_file_spasm()
{
}


void
srec_output_file_spasm::write(const srec_record &record)
{
	// This format can't do header records or termination records
	if (record.get_type() != srec_record::type_data)
		return;

	if (record.get_address() + record.get_length() > (1UL << 17))
	{
		fatal_error
		(
			"data address (0x%lX..0x%lX) too large",
			record.get_address(),
			record.get_address() + record.get_length() - 1
		);
	}

	long address = record.get_address();
	int j = 0;
	if (address & 1)
	{
		put_word(address++ / 2);
		put_char(' ');
		if (bigend)
		{
			put_byte(record.get_data(j++));
			put_byte(0xFF);
		}
		else
		{
			put_byte(0xFF);
			put_byte(record.get_data(j++));
		}
		put_char('\n');
	}

	while (j + 1 < record.get_length())
	{
		put_word(address / 2);
		put_char(' ');
		if (bigend)
		{
			put_byte(record.get_data(j + 1));
			put_byte(record.get_data(j));
		}
		else
		{
			put_byte(record.get_data(j));
			put_byte(record.get_data(j + 1));
		}
		put_char('\n');
		address += 2;
		j += 2;
	}

	if (j < record.get_length())
	{
		put_word(address / 2);
		put_char(' ');
		if (bigend)
		{
			put_byte(0xFF);
			put_byte(record.get_data(j));
		}
		else
		{
			put_byte(record.get_data(j));
			put_byte(0xFF);
		}
		put_char('\n');
	}
}


void
srec_output_file_spasm::line_length_set(int)
{
	//
	// Irrelevant.  Ignore.
	//
}


void
srec_output_file_spasm::address_length_set(int)
{
	//
	// Irrelevant.  Ignore.
	//
}


int
srec_output_file_spasm::preferred_block_size_get()
	const
{
	//
	// Irrelevant.  Use the largest we can get.
	// But make sure it is an even number of bytes long.
	//
	return (srec_record::max_data_length & ~1);
}
