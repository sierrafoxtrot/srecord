/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998, 1999, 2001 Peter Miller;
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
 * MANIFEST: functions to impliment the srec_output_file_srecord class
 */

#pragma implementation "srec_output_file_srecord"

#include <srec/output/file/srecord.h>
#include <srec/record.h>


srec_output_file_srecord::srec_output_file_srecord() :
	srec_output_file(),
	data_count(0),
	pref_block_size(32),
	address_length(2)
{
}


srec_output_file_srecord::srec_output_file_srecord(const char *filename) :
	srec_output_file(filename),
	data_count(0),
	pref_block_size(32),
	address_length(2)
{
}


srec_output_file_srecord::srec_output_file_srecord(const srec_output_file_srecord &) :
	srec_output_file(),
	data_count(0),
	pref_block_size(32),
	address_length(2)
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
		if (data_only_flag)
			break;
		write_inner(0, 0, 2, "HDR", 3);
		break;

	case srec_record::type_data:
		if
		(
			record.get_address() < (1UL << 16)
		&&
			address_length <= 2
		)
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
		else if
		(
			record.get_address() < (1UL << 24)
		&&
			address_length <= 3
		)
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

	case srec_record::type_start_address:
		if (data_only_flag)
			break;
		write_inner(5, data_count, 2, 0, 0);
		if
		(
			record.get_address() < (1UL << 16)
		&&
			address_length <= 2
		)
			write_inner(9, record.get_address(), 2, 0, 0);
		else if
		(
			record.get_address() < (1UL << 24)
		&&
			address_length <= 3
		)
			write_inner(8, record.get_address(), 3, 0, 0);
		else
			write_inner(7, record.get_address(), 4, 0, 0);
		break;

	case srec_record::type_unknown:
		fatal_error("can't write unknown record type");
	}
}


void
srec_output_file_srecord::line_length_set(int linlen)
{
	/*
	 * Given the number of characters, figure the maximum number of
	 * data baytes.
	 * 'S' <tag> <size1:2> <addr1:8> ...data... <cs1:2>
	 * 1 +  1 +  2 +       8 +       2*n +      2       <= linlen
	 */
	int n = (linlen - 14) / 2;

	/*
	 * Constrain based on the file format.
	 *
	 * The size field (max 255) includes the size of the data,
	 * the size of the address (up to 4 bytes) and the size of the
	 * size (1 byte), thus 250 (255 - 4 - 1) bytes of data is
	 * the safest maximum.	We could make it based on the address,
	 * but that's probably overkill.
	 */
	if (n < 1)
		n = 1;
	else if (n > 250)
		n = 250;

	/*
	 * An additional constraint is the size of the srec_record
	 * data buffer.
	 */
	if (n > srec_record::max_data_length)
		n = srec_record::max_data_length;
	pref_block_size = n;
}


void
srec_output_file_srecord::address_length_set(int n)
{
	if (n < 2)
		n = 2;
	else if (n > 4)
		n = 4;
	address_length = n;
}


int
srec_output_file_srecord::preferred_block_size_get()
	const
{
	return pref_block_size;
}
