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
 * MANIFEST: functions to impliment the srec_output_file_intel class
 */

#pragma implementation "srec_output_file_intel"

#include <srec/output/file/intel.h>
#include <srec/record.h>


srec_output_file_intel::srec_output_file_intel()
	: srec_output_file("-"), address_base(0)
{
}


srec_output_file_intel::srec_output_file_intel(const char *filename)
	: srec_output_file(filename), address_base(0)
{
}


srec_output_file_intel::srec_output_file_intel(const srec_output_file_intel &)
	: srec_output_file("-"), address_base(0)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_output_file_intel &
srec_output_file_intel::operator=(const srec_output_file_intel &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_output_file_intel::~srec_output_file_intel()
{
	write_inner(1, 0L, 0, 0);
}


void
srec_output_file_intel::write_inner(int tag, unsigned long address,
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
	put_char(':');
	checksum_reset();
	put_byte(data_nbytes);
	unsigned char tmp[2];
	srec_record::encode_big_endian(tmp, address, 2);
	put_byte(tmp[0]);
	put_byte(tmp[1]);
	put_byte(tag);
	const unsigned char *data_p = (const unsigned char *)data;
	for (int j = 0; j < data_nbytes; ++j)
		put_byte(data_p[j]);
	put_byte(-checksum_get());
	put_char('\n');
}


void
srec_output_file_intel::write(const srec_record &record)
{
	unsigned char tmp[4];
	switch (record.get_type())
	{
	case srec_record::type_header:
		/* ignore */
		break;

	case srec_record::type_data:
		if ((record.get_address() & 0xFFFF0000) != address_base)
		{
			address_base = record.get_address() & 0xFFFF0000;
			srec_record::encode_big_endian(tmp, record.get_address() >> 16, 2);
			write_inner(4, 0L, tmp, 2);
		}
		write_inner
		(
			0,
			record.get_address() & 0x0000FFFF,
			record.get_data(),
			record.get_length()
		);
		break;

	case srec_record::type_data_count:
		/* ignore */
		break;

	case srec_record::type_termination:
		srec_record::encode_big_endian(tmp, record.get_address(), 4);
		write_inner(5, 0L, tmp, 4);
		break;

	case srec_record::type_unknown:
		fatal_error("can't write unknown record type");
	}
}
