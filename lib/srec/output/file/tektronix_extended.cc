/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 2000, 2001 Peter Miller;
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
 * MANIFEST: functions to impliment the srec_output_file_tektronix_extended class
 */

#pragma implementation "srec_output_file_tektronix_extended"

#include <srec/output/file/tektronix_extended.h>
#include <srec/record.h>


srec_output_file_tektronix_extended::srec_output_file_tektronix_extended() :
	srec_output_file(),
	pref_block_size(32),
	address_length(4)
{
}


srec_output_file_tektronix_extended::srec_output_file_tektronix_extended(const char *filename) :
	srec_output_file(filename),
	pref_block_size(32),
	address_length(4)
{
}


srec_output_file_tektronix_extended::srec_output_file_tektronix_extended(const srec_output_file_tektronix_extended &) :
	srec_output_file(),
	pref_block_size(32),
	address_length(4)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_output_file_tektronix_extended &
srec_output_file_tektronix_extended::operator=(const srec_output_file_tektronix_extended &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_output_file_tektronix_extended::~srec_output_file_tektronix_extended()
{
	/* check for termination record */
}


void
srec_output_file_tektronix_extended::write_inner(int tag, unsigned long addr,
	int addr_nbytes, const void *data_p, int data_nbytes)
{
	if (addr_nbytes < address_length)
		addr_nbytes = address_length;
	unsigned char buf[260];
	int record_length = (addr_nbytes + data_nbytes) * 2 + 1;
	if (record_length >= 256)
		fatal_error("record too long (dmax=%d)", (254-2*addr_nbytes)/2);
	int csum = 0;
	int pos = 0;
	csum += buf[pos++] = (record_length >> 4) & 15;
	csum += buf[pos++] =  record_length       & 15;
	csum += buf[pos++] = tag;
	        buf[pos++] = 0; // checksum hi, fill in later
	        buf[pos++] = 0; // checksum lo, fill in later
	csum += buf[pos++] = addr_nbytes * 2;
	int j;
	for (j = 0; j < 2 * addr_nbytes; ++j)
		csum += buf[pos++] = (addr >> (4 * (2*addr_nbytes-1 - j))) & 15;
	const unsigned char *data = (const unsigned char *)data_p;
	for (j = 0; j < data_nbytes; ++j)
	{
		csum += buf[pos++] = (data[j] >> 4) & 15;
		csum += buf[pos++] =  data[j]       & 15;
	}

	// now insert the checksum...
	buf[3] = (csum >> 4) & 15;
	buf[4] =  csum       & 15;

	// emit the line
	put_char('%');
	for (j = 0; j < pos; ++j)
		put_nibble(buf[j]);
	put_char('\n');
}


static int
addr_width(unsigned long n)
{
	if (n < (1uL << 16))
		return 2;
	if (n < (1uL << 24))
		return 3;
	return 4;
}


void
srec_output_file_tektronix_extended::write(const srec_record &record)
{
	switch (record.get_type())
	{
	case srec_record::type_header:
		// Thios format can't do header records
		break;

	case srec_record::type_data:
		write_inner
		(
			6,
			record.get_address(),
			addr_width(record.get_address()),
			record.get_data(),
			record.get_length()
		);
		break;

	case srec_record::type_data_count:
		// ignore
		break;

	case srec_record::type_start_address:
		if (data_only_flag)
			break;
		write_inner
		(
			8,
			record.get_address(),
			addr_width(record.get_address()),
			0,
			0
		);
		break;

	case srec_record::type_unknown:
		fatal_error("can't write unknown record type");
	}
}


void
srec_output_file_tektronix_extended::line_length_set(int linlen)
{
	/*
	 * Given the number of characters, figure the maximum number of
	 * data baytes.
	 */
	int n = (linlen - 15) / 2;

	/*
	 * Constrain based on the file format.
	 *
	 * The size field (max 255 nibbles) includes the size of the
	 * data and the size of the address (up to 9 nibbles).	Thus 123
	 * ((255 - 9)/2) bytes of data is the safest maximum.  We could
	 * make it based on the address, but that's probably overkill.
	 */
	if (n < 1)
		n = 1;
	else if (n > 123)
		n = 123;

	/*
	 * An additional constraint is the size of the srec_record
	 * data buffer.
	 */
	if (n > srec_record::max_data_length)
		n = srec_record::max_data_length;
	pref_block_size = n;
}


void
srec_output_file_tektronix_extended::address_length_set(int n)
{
	if (n < 2)
		n = 2;
	if (n > 4)
		n = 4;
	address_length = n;
}


int
srec_output_file_tektronix_extended::preferred_block_size_get()
	const
{
	return pref_block_size;
}
