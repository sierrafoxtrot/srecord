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
 * MANIFEST: functions to impliment the srec_input_file_intel class
 */

#pragma implementation

#include <srec/input/file/intel.h>
#include <srec/record.h>


srec_input_file_intel::srec_input_file_intel()
	: srec_input_file(), data_record_count(0)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_input_file_intel::srec_input_file_intel(const srec_input_file_intel &)
	: srec_input_file(), data_record_count(0)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_input_file_intel::srec_input_file_intel(const char *filename)
	: srec_input_file(filename), data_record_count(0)
{
}


srec_input_file_intel &
srec_input_file_intel::operator=(const srec_input_file_intel &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_input_file_intel::~srec_input_file_intel()
{
	/* make sure the termination record is done */
}


int
srec_input_file_intel::read_inner(srec_record &record)
{
	int c = get_char();
	if (c < 0)
		return 0;
	if (c != ':')
		fatal_error("``:'' expected");
	unsigned char buffer[255+5];
	checksum_reset();
	buffer[0] = get_byte();
	buffer[1] = get_byte();
	buffer[2] = get_byte();
	buffer[3] = get_byte();
	for (int j = 0; j <= buffer[0]; ++j)
		buffer[4 + j] = get_byte();
	if (checksum_get() != 0x00)
		fatal_error("checksum mismatch (%02X)", checksum_get());
	c = get_char();
	if (c != '\n')
		fatal_error("end-of-line expected");

	srec_record::type type = srec_record::type_unknown;
	switch (buffer[3])
	{
	case 0:
		/* data */
		type = srec_record::type_data;
		break;

	case 1:
		/* termination */
		type = srec_record::type_termination;
		break;
	}
	record =
		srec_record
		(
			type,
			srec_record::decode_big_endian(buffer + 1, 2),
			buffer + 4,
			buffer[0]
		);
	return 1;
}


int
srec_input_file_intel::read(srec_record &record)
{
	for (;;)
	{
		if (!read_inner(record))
			return 0;
		switch (record.get_type())
		{
		case srec_record::type_unknown:
			fatal_error("record type not recognised");
			break;

		case srec_record::type_header:
			/* impossible */
			break;

		case srec_record::type_data:
			++data_record_count;
			if (record.get_length() == 0)
			{
				warning("empty data record ignored");
				continue;
			}
			break;

		case srec_record::type_data_count:
			/* impossible */
			continue;

		case srec_record::type_termination:
			if (record.get_length() > 0)
			{
				warning("data in termination record ignored");
				record.set_length(0);
			}
			break;
		}
		break;
	}
	return 1;
}
