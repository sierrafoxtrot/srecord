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
 * MANIFEST: functions to impliment the srec_input_file_ascii_hex class
 */

#pragma implementation "srec_input_file_ascii_hex"

#include <ctype.h>

#include <srec/input/file/ascii_hex.h>
#include <srec/record.h>


srec_input_file_ascii_hex::srec_input_file_ascii_hex()
	: srec_input_file(), garbage_warning(false),
		seen_some_input(false),
		address(0),
		state(state_initial)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_input_file_ascii_hex::srec_input_file_ascii_hex(const srec_input_file_ascii_hex &)
	: srec_input_file(), garbage_warning(false),
		seen_some_input(false),
		address(0),
		state(state_initial)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_input_file_ascii_hex::srec_input_file_ascii_hex(const char *filename)
	: srec_input_file(filename),
		garbage_warning(false), seen_some_input(false),
		address(0),
		state(state_initial)
{
}


srec_input_file_ascii_hex &
srec_input_file_ascii_hex::operator=(const srec_input_file_ascii_hex &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_input_file_ascii_hex::~srec_input_file_ascii_hex()
{
	/* check termination? */
}


int
srec_input_file_ascii_hex::read_inner(srec_record &record)
{
	if (state == state_ignore)
		return 0;

	if (state == state_initial)
	{
		for (;;)
		{
			int c = get_char();
			if (c < 0)
			{
				state = state_ignore;
				return 0;
			}
			if (c == 2)
			{
				state = state_body;
				break;
			}
			if (!garbage_warning)
			{
				warning("ignoring garbage lines");
				garbage_warning = true;
			}
		}
	}

	for (;;)
	{
		int c = peek_char();
		if (c < 0)
		{
			return 0;
		}
		if (isxdigit(c))
		{
			unsigned char c = get_byte();
			record =
				srec_record
				(
					srec_record::type_data,
					address,
					&c,
					1
				);
			++address;
			return 1;
		}
		c = get_char();
		switch (c)
		{
		case 3:
			state = state_ignore;
			return 0;

		case ' ':
		case '\t':
		case '\r':
		case '\n':
		case '\f':
			break;

		default:
			fatal_error("illegal character");

		case '$':
			if (get_char() != 'A')
				fatal_error("unknown command");
			address = 0;
			int n = 0;
			for (n = 0; n < 4 && peek_char() != ','; ++n)
			{
				int b = get_byte();
				address = (address << 8) + b;
			}
			if (n < 2)
				fatal_error("short address");
			if (get_char() != ',')
				fatal_error("comma expected");
			break;
		}
	}
}


int
srec_input_file_ascii_hex::read(srec_record &record)
{
	if (!read_inner(record))
	{
		if (!seen_some_input)
			fatal_error("file contains no data");
		return 0;
	}
	seen_some_input = true;
	return 1;
}
