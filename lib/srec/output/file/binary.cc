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
 * MANIFEST: functions to impliment the srec_output_file_binary class
 */

#pragma implementation "srec_output_file_binary"

#include <srec/output/file/binary.h>
#include <srec/record.h>


srec_output_file_binary::srec_output_file_binary()
	: srec_output_file("-")
{
}


srec_output_file_binary::srec_output_file_binary(const char *filename)
	: srec_output_file(filename)
{
}


srec_output_file_binary::srec_output_file_binary(const srec_output_file_binary &)
	: srec_output_file("-")
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_output_file_binary &
srec_output_file_binary::operator=(const srec_output_file_binary &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_output_file_binary::~srec_output_file_binary()
{
}


void
srec_output_file_binary::write(const srec_record &record)
{
	if (record.get_type() != srec_record::type_data)
		return;
	seek_to(record.get_address());
	const unsigned char *data = record.get_data();
	int length = record.get_length();
	while (length-- > 0)
		put_char(*data++);
}
