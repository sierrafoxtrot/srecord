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
 * MANIFEST: functions to impliment the srec_output_file_tektronix class
 */

#pragma implementation

#include <srec/output/file/tektronix.h>
#include <srec/record.h>


srec_output_file_tektronix::srec_output_file_tektronix()
	: srec_output_file("-")
{
}


srec_output_file_tektronix::srec_output_file_tektronix(const char *filename)
	: srec_output_file(filename)
{
}


srec_output_file_tektronix::srec_output_file_tektronix(const srec_output_file_tektronix &)
	: srec_output_file("-")
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_output_file_tektronix &
srec_output_file_tektronix::operator=(const srec_output_file_tektronix &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_output_file_tektronix::~srec_output_file_tektronix()
{
	/* make sure terminator is written */
}


void
srec_output_file_tektronix::write_inner(unsigned long address,
	const void *data, int data_nbytes)
{
	/*
	 * Make sure the line is not too long.
	 */
	if (data_nbytes >= 256)
		fatal_error("data length (%d) too long", data_nbytes);

	/*
	 * Emit the line as hexadecimal text.
	 */
	put_char('/');
	unsigned char tmp[2];
	srec_record::encode_big_endian(tmp, address, 2);
	put_byte(tmp[0]);
	put_byte(tmp[1]);
	put_byte(data_nbytes);
	put_byte
	(
		(tmp[0] >> 4) + (tmp[0] & 15) +
		(tmp[1] >> 4) + (tmp[1] & 15) +
		(data_nbytes >> 4) + (data_nbytes & 15)
	);
	checksum_reset();
	if (data_nbytes)
	{
		const unsigned char *data_p = (const unsigned char *)data;
		for (int j = 0; j < data_nbytes; ++j)
			put_byte(data_p[j]);
		put_byte(checksum_get());
	}
	put_char('\n');
}


void
srec_output_file_tektronix::write(const srec_record &record)
{
	switch (record.get_type())
	{
	case srec_record::type_header:
		/* ignore */
		break;

	case srec_record::type_data:
		if (record.get_length() == 0)
			break; /* ignore */
		if (record.get_address() >= (1UL << 16))
		{
			fatal_error
			(
				"data address (%08lX) too large",
				record.get_address()
			);
		}
		write_inner
		(
			record.get_address(),
			record.get_data(),
			record.get_length()
		);
		break;

	case srec_record::type_data_count:
		/* ignore */
		break;

	case srec_record::type_termination:
		if (record.get_address() >= (1UL << 16))
		{
			fatal_error
			(
				"termination address (%08lX) too large",
				record.get_address()
			);
		}
		write_inner(record.get_address(), 0, 0);
		break;

	case srec_record::type_unknown:
		fatal_error("can't write unknown record type");
	}
}
