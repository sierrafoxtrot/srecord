//
//	srecord - manipulate eprom load files
//	Copyright (C) 2001, 2002 Peter Miller;
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
// MANIFEST: functions to impliment the srec_input_file_signetics class
//

#pragma implementation "srec_input_file_signetics"

#include <srec/input/file/signetics.h>
#include <srec/record.h>


srec_input_file_signetics::srec_input_file_signetics() :
	srec_input_file(),
	garbage_warning(false),
	seen_some_input(false)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_input_file_signetics::srec_input_file_signetics(const srec_input_file_signetics &) :
	srec_input_file(),
	garbage_warning(false),
	seen_some_input(false)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_input_file_signetics::srec_input_file_signetics(const char *filename) :
	srec_input_file(filename),
	garbage_warning(false),
	seen_some_input(false)
{
}


srec_input_file_signetics &
srec_input_file_signetics::operator=(const srec_input_file_signetics &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_input_file_signetics::~srec_input_file_signetics()
{
}


void
srec_input_file_signetics::checksum_add(unsigned char n)
{
	checksum ^= n;
	checksum = (checksum << 1) | ((checksum >> 7) & 1);
}


int
srec_input_file_signetics::read_inner(srec_record &record)
{
	for (;;)
	{
		int c = get_char();
		if (c < 0)
			return 0;
		if (c == ':')
			break;
		if (c == '\n')
			continue;
		if (!garbage_warning)
		{
			warning("ignoring garbage lines");
			garbage_warning = true;
		}
		for (;;)
		{
			c = get_char();
			if (c < 0)
				return 0;
			if (c == '\n')
				break;
		}
	}
	checksum_reset();
	long address = get_word();
	int length = get_byte();
	if (length == 0)
	{
		// this is the end indicator
		if (get_char() != '\n')
			fatal_error("end-of-line expected");
		return 0;
	}

	int running_checksum = checksum_get();
	if (running_checksum != get_byte())
		fatal_error("address checksum mismatch");

	checksum_reset();
	unsigned char buffer[256];
	for (int j = 0; j < length; ++j)
		buffer[j] = get_byte();

	running_checksum = checksum_get();
	if (running_checksum != get_byte())
		fatal_error("data checksum mismatch");

	if (get_char() != '\n')
		fatal_error("end-of-line expected");

	srec_record::type_t type = srec_record::type_data;
	record = srec_record(type, address, buffer, length);
	return 1;
}


int
srec_input_file_signetics::read(srec_record &record)
{
	if (!read_inner(record))
	{
		if (!seen_some_input && garbage_warning)
			fatal_error("file contains no data");
		return 0;
	}
	seen_some_input = true;
	return 1;
}


const char *
srec_input_file_signetics::get_file_format_name()
	const
{
	return "Signetics";
}
