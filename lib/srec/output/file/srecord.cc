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
 * MANIFEST: functions to impliment the srec_output_file_srecord class
 */

#pragma implementation

#include <srec/output/file/srecord.h>
#include <srec/record.h>


srec_output_file_srecord::srec_output_file_srecord()
	: srec_output_file(), data_count(0)
{
}


srec_output_file_srecord::srec_output_file_srecord(const char *filename)
	: srec_output_file(filename), data_count(0)
{
}


srec_output_file_srecord::srec_output_file_srecord(const srec_output_file_srecord &)
	: srec_output_file(), data_count(0)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_output_file_srecord &
srec_output_file_srecord::operator=(const srec_output_file_srecord &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_output_file_srecord::~srec_output_file_srecord()
{
	/* check for data count record */
	/* check for termination record */
}


void
srec_output_file_srecord::write_inner(int tag, unsigned long address,
	int address_nbytes, const void *data, int data_nbytes)
{
	/*
	 * Make sure the line is not too long.
	 */
	if (address_nbytes + data_nbytes > 254)
	{
		fatal_error
		(
			"data length (%d+%d) too long",
			address_nbytes,
			data_nbytes
		);
	}

	/*
	 * Assemble the data for this line.
	 */
	unsigned char buffer[256];
	int line_length = address_nbytes + data_nbytes + 1;
	buffer[0] = line_length;
	srec_record::encode_big_endian(buffer + 1, address, address_nbytes);
	if (data_nbytes)
		memcpy(buffer + 1 + address_nbytes, data, data_nbytes);

	/*
	 * Emit the line as hexadecimal text.
	 */
	put_char('S');
	put_nibble(tag);
	checksum_reset();
	for (int j = 0; j < line_length; ++j)
		put_byte(buffer[j]);
	put_byte(~checksum_get());
	put_char('\n');
}


void
srec_output_file_srecord::write(const srec_record &record)
{
	switch (record.get_type())
	{
	case srec_record::type_header:
		write_inner(0, 0, 2, "HDR", 3);
		break;

	case srec_record::type_data:
		if (record.get_address() < (1UL << 16))
		{
			write_inner
			(
				1,
				record.get_address(),
				2,
				record.get_data(),
				record.get_length()
			);
		}
		else if (record.get_address() < (1UL << 24))
		{
			write_inner
			(
				2,
				record.get_address(),
				3,
				record.get_data(),
				record.get_length()
			);
		}
		else
		{
			write_inner
			(
				3,
				record.get_address(),
				4,
				record.get_data(),
				record.get_length()
			);
		}
		++data_count;
		break;

	case srec_record::type_data_count:
		/* ignore */
		break;

	case srec_record::type_termination:
		write_inner(5, data_count, 2, 0, 0);
		if (record.get_address() < (1UL << 16))
			write_inner(9, record.get_address(), 2, 0, 0);
		else if (record.get_address() < (1UL << 24))
			write_inner(8, record.get_address(), 3, 0, 0);
		else
			write_inner(7, record.get_address(), 4, 0, 0);
		break;

	case srec_record::type_unknown:
		fatal_error("can't write unknown record type");
	}
}
