//
//	srecord - manipulate eprom load files
//	Copyright (C) 2001, 2002, 2006 Peter Miller;
//	All rights reserved.
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
// MANIFEST: functions to impliment the srec_output_file_signetics class
//

#pragma implementation "srec_output_file_signetics"

#include <srec/output/file/signetics.h>
#include <srec/record.h>


srec_output_file_signetics::srec_output_file_signetics() :
    srec_output_file(),
    pref_block_size(32),
    last_address(0)
{
}


srec_output_file_signetics::srec_output_file_signetics(const char *filename) :
    srec_output_file(filename),
    pref_block_size(32),
    last_address(0)
{
}


srec_output_file_signetics::srec_output_file_signetics(
	const srec_output_file_signetics &) :
    srec_output_file(),
    pref_block_size(32),
    last_address(0)
{
    fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_output_file_signetics &
srec_output_file_signetics::operator=(const srec_output_file_signetics &)
{
    fatal_error("bug (%s, %d)", __FILE__, __LINE__);
    return *this;
}


srec_output_file_signetics::~srec_output_file_signetics()
{
    put_char(':');
    put_word(last_address);
    put_byte(0);
    put_char('\n');
}


void
srec_output_file_signetics::checksum_add(unsigned char n)
{
    checksum ^= n;
    checksum = (checksum << 1) | ((checksum >> 7) & 1);
}


void
srec_output_file_signetics::write(const srec_record &record)
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
	put_char(':');
	checksum_reset();
	put_word(record.get_address());
	put_byte(record.get_length());
	put_byte(checksum_get());
	checksum_reset();
	for (int j = 0; j < record.get_length(); ++j)
	    put_byte(record.get_data(j));
	put_byte(checksum_get());
	put_char('\n');
	last_address = record.get_address() + record.get_length();
	break;

    case srec_record::type_data_count:
	// ignore
	break;

    case srec_record::type_start_address:
	// This format can't do start address records
	if (data_only_flag)
	    break;
	break;

    case srec_record::type_unknown:
	fatal_error("can't write unknown record type");
    }
}


void
srec_output_file_signetics::line_length_set(int linlen)
{
    //
    // Given the number of characters, figure the maximum number of
    // data baytes.
    //
    int             n = (linlen - 11) / 2;

    //
    // Constrain based on the file format.
    //
    if (n < 1)
	n = 1;
    else if (n > 255)
	n = 255;

    //
    // An additional constraint is the size of the srec_record
    // data buffer.
    //
    if (n > srec_record::max_data_length)
	n = srec_record::max_data_length;
    pref_block_size = n;
}


void
srec_output_file_signetics::address_length_set(int)
{
    // ignore (this is only a 16-bit format)
}


int
srec_output_file_signetics::preferred_block_size_get() const
{
    return pref_block_size;
}
