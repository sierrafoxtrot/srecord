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
 * MANIFEST: functions to impliment the srec_input_file_srecord class
 */

#pragma implementation

#include <srec/input/file/srecord.h>
#include <srec/record.h>


srec_input_file_srecord::srec_input_file_srecord()
	: srec_input_file(), data_record_count(0)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_input_file_srecord::srec_input_file_srecord(const srec_input_file_srecord &)
	: srec_input_file(), data_record_count(0)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_input_file_srecord::srec_input_file_srecord(const char *filename)
	: srec_input_file(filename), data_record_count(0)
{
}


srec_input_file_srecord &
srec_input_file_srecord::operator=(const srec_input_file_srecord &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_input_file_srecord::~srec_input_file_srecord()
{
	/* make sure the count record is done */
	/* make sure the termination record is done */
}


int
srec_input_file_srecord::read_inner(srec_record &record)
{
	int c = get_char();
	if (c < 0)
		return 0;
	if (c != 'S')
		fatal_error("``S'' expected");
	int tag = get_nibble();
	int line_length = get_byte();
	if (line_length < 1)
		fatal_error("line length invalid");
	unsigned char buffer[256];
	int checksum = line_length;
	for (int j = 0; j < line_length; ++j)
	{
		c = get_byte();
		buffer[j] = c;
		checksum += c;
	}
	checksum &= 0xFF;
	if (checksum != 0xFF)
		fatal_error("checksum mismatch (%02X)", checksum);
	c = get_char();
	if (c != '\n')
		fatal_error("end-of-line expected");
	--line_length;

	int naddr = 2;
	srec_record::type type = srec_record::type_unknown;
	switch (tag)
	{
	case 0:
		/* header */
		type = srec_record::type_header;
		if (line_length < naddr)
		{
			/* Some programs write Very short headers. */
			naddr = line_length;
		}
		break;

	case 1:
		/* data */
		type = srec_record::type_data;
		break;

	case 2:
		/* data */
		type = srec_record::type_data;
		naddr = 3;
		break;

	case 3:
		/* data */
		type = srec_record::type_data;
		naddr = 4;
		break;

	case 5:
		/* data count */
		type = srec_record::type_data_count;
		break;

	case 7:
		/* termination */
		type = srec_record::type_termination;
		naddr = 4;
		break;

	case 8:
		/* termination */
		type = srec_record::type_termination;
		naddr = 3;
		break;

	case 9:
		/* termination */
		type = srec_record::type_termination;
		break;
	}
	if (line_length < naddr)
	{
		fatal_error
		(
			"data length too short (%d < %d) for data type (%x)",
			line_length,
			naddr,
			tag
		);
	}
	record =
		srec_record
		(
			type,
			srec_record::decode_big_endian(buffer, naddr),
			buffer + naddr,
			line_length - naddr
		);
	return 1;
}


int
srec_input_file_srecord::read(srec_record &record)
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
			if (record.get_address())
			{
				warning("address in header record ignored");
				record.set_address(0);
			}
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
			if (record.get_address() != (unsigned long)(data_record_count & 0xFFFF))
			{
				fatal_error
				(
			       "data record count mismatch (file %ld, read %d)",
					record.get_address(),
					data_record_count
				);
			}
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
