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
 * MANIFEST: functions to impliment the srec_input_file_binary class
 */

#pragma implementation "srec_input_file_binary"

#include <srec/input/file/binary.h>
#include <srec/record.h>


srec_input_file_binary::srec_input_file_binary()
	: srec_input_file(), address(0)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_input_file_binary::srec_input_file_binary(const srec_input_file_binary &)
	: srec_input_file(), address(0)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_input_file_binary::srec_input_file_binary(const char *filename)
	: srec_input_file(filename), address(0)
{
}


srec_input_file_binary &
srec_input_file_binary::operator=(const srec_input_file_binary &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_input_file_binary::~srec_input_file_binary()
{
}


int
srec_input_file_binary::read(srec_record &record)
{
	int c = get_char();
	if (c < 0)
		return 0;
	int length = 0;
	unsigned char data[256];
	for (;;)
	{
		data[length++] = c;
		if (length >= (int)sizeof(data))
			break;
		c = get_char();
		if (c < 0)
			break;
	}
	record = srec_record(srec_record::type_data, address, data, length);
	address += length;
	return 1;
}
