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
 * MANIFEST: functions to impliment the srec_input_file_tektronix class
 */

#pragma implementation "srec_input_file_tektronix"

#include <srec/input/file/tektronix.h>
#include <srec/record.h>


srec_input_file_tektronix::srec_input_file_tektronix()
	: srec_input_file(), data_record_count(0), garbage_warning(false),
		seen_some_input(false), termination_seen(false)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_input_file_tektronix::srec_input_file_tektronix(const srec_input_file_tektronix &)
	: srec_input_file(), data_record_count(0), garbage_warning(false),
		seen_some_input(false), termination_seen(false)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_input_file_tektronix::srec_input_file_tektronix(const char *filename)
	: srec_input_file(filename), data_record_count(0),
		garbage_warning(false), seen_some_input(false),
		termination_seen(false)
{
}


srec_input_file_tektronix &
srec_input_file_tektronix::operator=(const srec_input_file_tektronix &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_input_file_tektronix::~srec_input_file_tektronix()
{
	/* make sure the termination record is done */
}


int
srec_input_file_tektronix::read_inner(srec_record &record)
{
	for (;;)
	{
		int c = get_char();
		if (c < 0)
			return 0;
		if (c == '/')
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

	if (peek_char() == '/')
	{
		/*
		 * lines beginning with ``//'' are end-of-file records
		 */
		seek_to_end();
		return 0;
	}

	unsigned char buffer[255+5];
	checksum_reset();
	buffer[0] = get_byte();
	buffer[1] = get_byte();
	buffer[2] = get_byte();
	buffer[3] = get_byte();
	int nibble_checksum =
		(buffer[0] >> 4) + (buffer[0] & 15) +
		(buffer[1] >> 4) + (buffer[1] & 15) +
		(buffer[2] >> 4) + (buffer[2] & 15);
	if (nibble_checksum != buffer[3])
	{
		fatal_error
		(
			"header checksum mismatch (%02X != %02X)",
			nibble_checksum,
			buffer[3]
		);
	}
	if (buffer[2])
	{
		checksum_reset();
		for (int j = 0; j < buffer[2]; ++j)
			buffer[4 + j] = get_byte();
		int data_checksum_calc = checksum_get();
		int data_checksum_file = get_byte();
		if (data_checksum_calc != data_checksum_file)
		{
			fatal_error
			(
				"data checksum mismatch (%02X != %02X)",
				data_checksum_calc,
				data_checksum_file
			);
		}
	}
	if (get_char() != '\n')
		fatal_error("end-of-line expected");

	record =
		srec_record
		(
			(
				buffer[2] == 0
			?
				srec_record::type_termination
			:
				srec_record::type_data
			),
			srec_record::decode_big_endian(buffer, 2),
			buffer + 4,
			buffer[2]
		);
	return 1;
}


int
srec_input_file_tektronix::read(srec_record &record)
{
	for (;;)
	{
		if (!read_inner(record))
		{
			if (!seen_some_input && garbage_warning)
				fatal_error("file contains no data");
			if (data_record_count <= 0)
				fatal_error("file contains no data");
			if (!termination_seen)
			{
				warning("no termination record");
				termination_seen = true;
			}
			return 0;
		}
		seen_some_input = true;
		if
		(
			record.get_type() != srec_record::type_termination
		&&
			termination_seen
		)
		{
			warning("termination record should be last");
			termination_seen = false;
		}
		switch (record.get_type())
		{
		default:
			/* impossible */
			continue;

		case srec_record::type_data:
			++data_record_count;
			break;

		case srec_record::type_termination:
			if (termination_seen)
				warning("redundant termination record");
			termination_seen = true;
			break;
		}
		break;
	}
	return 1;
}
