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
 * MANIFEST: functions to impliment the srec_output_file_ti_tagged class
 */

#pragma implementation "srec_output_file_ti_tagged"

#include <srec/output/file/ti_tagged.h>
#include <srec/record.h>


srec_output_file_ti_tagged::srec_output_file_ti_tagged()
	: srec_output_file(),
	  address(0),
	  line_length(74)
{
}


srec_output_file_ti_tagged::srec_output_file_ti_tagged(const char *filename)
	: srec_output_file(filename),
	  address(0),
	  line_length(74)
{
}


srec_output_file_ti_tagged::srec_output_file_ti_tagged(const srec_output_file_ti_tagged &)
	: srec_output_file(),
	  address(0),
	  line_length(74)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_output_file_ti_tagged &
srec_output_file_ti_tagged::operator=(const srec_output_file_ti_tagged &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_output_file_ti_tagged::~srec_output_file_ti_tagged()
{
	/* check for termination record */
}


void
srec_output_file_ti_tagged::put_char(int c)
{
	if (c == '\n')
	{
		csum = 0;
		column = 0;
	}
	else
	{
		csum += (unsigned char)c;
		++column;
	}
	inherited::put_char(c);
}

void
srec_output_file_ti_tagged::put_eoln()
{
	put_char('7');
	put_word(-csum);
	put_char('F');
	put_char('\n');
}


void
srec_output_file_ti_tagged::write(const srec_record &record)
{
	int pos = 0;
	switch (record.get_type())
	{
	case srec_record::type_header:
		if (data_only_flag)
			break;
		put_string("K0005");
		break;

	case srec_record::type_data:
		if (address != record.get_address())
		{
			if (column + 5 > line_length)
				put_eoln();
			address = record.get_address();
			put_char('9');
			put_word(address);
		}
		pos = 0;
		for (; pos + 2 <= record.get_length(); pos += 2)
		{
			if (column + 5 > line_length)
				put_eoln();
			put_char('B');
			put_byte(record.get_data(pos));
			put_byte(record.get_data(pos + 1));
			address += 2;
		}
		for (; pos < record.get_length(); ++pos)
		{
			if (column + 3 > line_length)
				put_eoln();
			put_char('*');
			put_byte(record.get_data(pos));
			++address;
		}
		break;

	case srec_record::type_data_count:
		/* ignore */
		break;

	case srec_record::type_termination:
		if (column)
			put_eoln();
		if (data_only_flag)
			break;
		put_char(':');
		put_char('\n');
		break;

	case srec_record::type_unknown:
		fatal_error("can't write unknown record type");
	}
}


void
srec_output_file_ti_tagged::line_length_set(int linlen)
{
	// reduce the line length by 6 characters, to account for the
	// checksum, so we don't keep subtracting it later.
	line_length = linlen - 6;

	// make sure the line is long enoght to do anything useful
	if (line_length < 5)
		line_length = 5;
}


void
srec_output_file_ti_tagged::address_length_set(int)
{
	// ignore (this is a 16-bit format)
}


int
srec_output_file_ti_tagged::preferred_block_size_get()
	const
{
	int n = (line_length / 5) * 2;
	if (n < 1)
		n = 1;
	if (n > srec_record::max_data_length)
		n = srec_record::max_data_length;
	return n;
}
