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
 * MANIFEST: functions to impliment the srec_output_file_c class
 */

#pragma implementation "srec_output_file_c"

#include <interval.h>
#include <srec/output/file/c.h>
#include <srec/record.h>


srec_output_file_c::srec_output_file_c()
	: srec_output_file("-"), prefix("no_name_given"), line_length(75)
{
}


srec_output_file_c::srec_output_file_c(const char *filename, const char *a2)
	: srec_output_file(filename), prefix(a2), line_length(75)
{
}


srec_output_file_c::srec_output_file_c(const srec_output_file_c &)
	: srec_output_file("-"), line_length(75)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_output_file_c &
srec_output_file_c::operator=(const srec_output_file_c &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


void
srec_output_file_c::emit_header()
{
	if (header_done)
		return;
	put_stringf("unsigned char %s[] =\n{\n", prefix.c_str());
	header_done = true;
}


void
srec_output_file_c::emit_byte(int n)
{
	if (column >= line_length)
	{
		put_char('\n');
		column = 0;
	}
	else if (column)
	{
		put_char(' ');
		++column;
	}
	put_stringf("0x%02X,", (unsigned char)n);
	column += 5;
}


srec_output_file_c::~srec_output_file_c()
{
	emit_header();
	if (range.empty())
		emit_byte(0);
	if (column)
		put_char('\n');
	put_string("};\n");

	if (!data_only_flag)
	{
		put_stringf("unsigned long %s_termination = 0x%08lX;\n",
			prefix.c_str(), taddr);
		put_stringf("unsigned long %s_start       = 0x%08lX;\n",
			prefix.c_str(), range.get_lowest());
		put_stringf("unsigned long %s_finish      = 0x%08lX;\n",
			prefix.c_str(), range.get_highest());
	}
	put_stringf("unsigned long %s_length      = 0x%08lX;\n",
		prefix.c_str(), range.get_highest() - range.get_lowest());
}


void
srec_output_file_c::write(const srec_record &record)
{
	switch (record.get_type())
	{
	default:
		/* ignore */
		break;

	case srec_record::type_data:
		if (range.empty())
			current_address = record.get_address();
		range +=
			interval
			(
				record.get_address(),
				record.get_address() + record.get_length()
			);
		emit_header();
		while (current_address < record.get_address())
		{
			put_byte(0xFF);
			++current_address;
		}
		for (int j = 0; j < record.get_length(); ++j)
		{
			if (record.get_address() + j < current_address)
				continue;
			emit_byte(record.get_data(j));
			++current_address;
		}
		break;

	case srec_record::type_termination:
		taddr = record.get_address();
		break;
	}
}


void
srec_output_file_c::line_length_set(int n)
{
	n = (n + 1) / 6;
	if (n < 1)
		n = 1;
	n = n * 6 - 1;
	line_length = n;
}


int
srec_output_file_c::preferred_block_size_get()
	const
{
	/*
	 * Irrelevant.  Use the largest we can get.
	 */
	return srec_record::max_data_length;
}
