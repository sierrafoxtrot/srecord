/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 2000 Peter Miller;
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
 * MANIFEST: functions to impliment the srec_input_file_mos_tech class
 */

#pragma implementation "srec_input_file_mos_tech"

#include <srec/input/file/mos_tech.h>
#include <srec/record.h>


srec_input_file_mos_tech::srec_input_file_mos_tech()
	: srec_input_file(),
	  garbage_warning(false),
	  seen_some_input(false)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_input_file_mos_tech::srec_input_file_mos_tech(const srec_input_file_mos_tech &)
	: srec_input_file(),
	  garbage_warning(false),
	  seen_some_input(false)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_input_file_mos_tech::srec_input_file_mos_tech(const char *filename)
	: srec_input_file(filename),
	  garbage_warning(false),
	  seen_some_input(false)
{
}


srec_input_file_mos_tech &
srec_input_file_mos_tech::operator=(const srec_input_file_mos_tech &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_input_file_mos_tech::~srec_input_file_mos_tech()
{
}


int
srec_input_file_mos_tech::read_inner(srec_record &record)
{
	for (;;)
	{
		int c = get_char();
		if (c < 0)
			return 0;
		if (c == ';')
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
	int length = get_byte();
cerr << __FILE__ << ": " << __LINE__ << ": length=" << length << endl;
	if (length == 0)
	{
		if (get_char() != '\n')
			fatal_error("end-of-line expected");
		while (get_char() >= 0)
			;
		return 0;
	}

	unsigned long address = get_word();
	unsigned char buffer[256];
	for (int j = 0; j < length; ++j)
		buffer[j] = get_byte();
	int csumX = checksum_get16();
	int csum = get_word();
	if (csumX != csum)
		fatal_error("checksum mismatch (%04X)", csumX);
	if (get_char() != '\n')
		fatal_error("end-of-line expected");

	srec_record::type type = srec_record::type_data;
	record = srec_record(type, address, buffer, length);
	return 1;
}


int
srec_input_file_mos_tech::read(srec_record &record)
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
