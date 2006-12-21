//
//	srecord - manipulate eprom load files
//	Copyright (C) 2001, 2002, 2006 Peter Miller
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: functions to impliment the srec_output_file_dec_binary class
//


#include <stdio.h> // HACK
#include <lib/srec/output/file/dec_binary.h>
#include <lib/srec/record.h>

#define BLOCK_SIZE 512
#define BLOCK_SIZE_MASK (BLOCK_SIZE - 1)

srec_output_file_dec_binary::srec_output_file_dec_binary() :
	srec_output_file(),
	pref_block_size(preferred_block_size_calculate())
{
}


srec_output_file_dec_binary::srec_output_file_dec_binary(const char *filename) :
	srec_output_file(filename),
	pref_block_size(preferred_block_size_calculate())
{
}


srec_output_file_dec_binary::srec_output_file_dec_binary(const srec_output_file_dec_binary &) :
	srec_output_file(),
	pref_block_size(preferred_block_size_calculate())
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_output_file_dec_binary &
srec_output_file_dec_binary::operator=(const srec_output_file_dec_binary &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_output_file_dec_binary::~srec_output_file_dec_binary()
{
}


void
srec_output_file_dec_binary::put_byte(unsigned char n)
{
	checksum_add(n);
	put_char(n);
	byte_offset++;
}


void
srec_output_file_dec_binary::put_word(int n)
{
	put_byte(n);
	put_byte(n >> 8);
}


void
srec_output_file_dec_binary::write(const srec_record &record)
{
	switch (record.get_type())
	{
	case srec_record::type_header:
		// This format can't do header records
		break;

	case srec_record::type_data:
		if (record.get_length() < 1)
			return;
		if (record.get_address() + record.get_length() > (1UL << 16))
		{
			fatal_error
			(
				"data address (0x%lX..0x%lX) too large",
				record.get_address(),
				record.get_address() + record.get_length() - 1
			);
		}

#if 0
		//
		// Don't span block boundaries.
		//
		if
		(
			(byte_offset & ~BLOCK_SIZE_MASK)
		!=
			((byte_offset + 6 + record.get_length())
				& ~BLOCK_SIZE_MASK)
		)
		{
fprintf(stderr, "%s: %d: spans block %4.4lX/%4.4lX\n", __FILE__, __LINE__, byte_offset, byte_offset + 6 + record.get_length());
			while (byte_offset & BLOCK_SIZE_MASK)
				put_byte(0);
		}
#endif

		//
		// Write the data out.
		//
		checksum_reset();
		put_word(1);
		put_word(record.get_length() + 6);
		put_word(record.get_address());
		for (int j = 0; j < record.get_length(); ++j)
			put_byte(record.get_data(j));
		put_byte(-checksum_get());
		if (byte_offset & 1)
			put_byte(0);
		break;

	case srec_record::type_data_count:
		// ignore
		break;

	case srec_record::type_start_address:
		if (data_only_flag)
			break;
		checksum_reset();
		put_word(1);
		put_word(6);
		put_word(record.get_address());
		put_byte(-checksum_get());

		// Round off to a whole multiple of BLOCK_SIZE bytes.
		while (byte_offset & BLOCK_SIZE_MASK)
			put_byte(0);
		break;

	case srec_record::type_unknown:
		fatal_error("can't write unknown record type");
	}
}


void
srec_output_file_dec_binary::line_length_set(int)
{
	// Ignore.
}


void
srec_output_file_dec_binary::address_length_set(int)
{
	// Ignore (this is only a 16-bit format).
}


int
srec_output_file_dec_binary::preferred_block_size_get()
	const
{
	return pref_block_size;
}


int
srec_output_file_dec_binary::preferred_block_size_calculate()
{
#if 0
	//
	// Because we don't cross BLOCK_SIZE boundaries, we need a number
	// less than srec_record::max_data_length that, when you add 7
	// and double, gives
	//
	int max = BLOCK_SIZE;
	for (;;)
	{
	    if (max - 7 <= srec_record::max_data_length)
		return (max - 7);
	    max >>= 1;
	}
	return 1;
#else
	return srec_record::max_data_length;
#endif
}


const char *
srec_output_file_dec_binary::mode()
	const
{
	return "wb";
}
