//
//	srecord - manipulate eprom load files
//	Copyright (C) 2003 Peter Miller;
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
// MANIFEST: functions to impliment the srec_output_file_intel16 class
//

#pragma implementation "srec_output_file_intel16"

#include <srec/output/file/intel16.h>
#include <srec/record.h>


srec_output_file_intel16::srec_output_file_intel16(const char *filename) :
    srec_output_file(filename),
    address_base(0),
    pref_block_size(32)
{
    // The address base always has the lower 16 bits set to zero.
    // By making it be 1, we force the first data record to emit an
    // address base record first.
}


srec_output_file_intel16::~srec_output_file_intel16()
{
    if (!data_only_flag)
       	write_inner(1, 0L, 0, 0);
}


void
srec_output_file_intel16::write_inner(int tag, unsigned long address,
    const void *data, int data_nbytes)
{
    //
    // Make sure the line is not too long.
    //
    if (data_nbytes >= 255*2)
	fatal_error("data length (%d) too long", data_nbytes);

    //
    // Emit the line as hexadecimal text.
    //
    put_char(':');
    checksum_reset();
    int even_nbytes = (data_nbytes & 1) ? (data_nbytes + 1) : data_nbytes;
    put_byte(even_nbytes >> 1);
    unsigned char tmp[2];
    srec_record::encode_big_endian(tmp, address, 2);
    put_byte(tmp[0]);
    put_byte(tmp[1]);
    put_byte(tag);
    const unsigned char *data_p = (const unsigned char *)data;
    for (int j = 0; j < even_nbytes; ++j)
    {
	// Note: bytes are ordered HI,LO so we invert LSB
	// Watch out for odd record lengths.
	if ((j ^ 1) >= data_nbytes)
	    put_byte(0xFF);
	else
	    put_byte(data_p[j ^ 1]);
    }
    put_byte(-checksum_get());
    put_char('\n');
}


void
srec_output_file_intel16::write(const srec_record &record)
{
    unsigned char tmp[4];
    switch (record.get_type())
    {
    case srec_record::type_header:
	//
	// This format can't do header records
	//
	break;

    case srec_record::type_data:
	if ((record.get_address() & 0xFFFE0000) != address_base)
	{
	    address_base = record.get_address() & 0xFFFE0000;
	    srec_record::encode_big_endian(tmp, record.get_address() >> 17, 2);
	    write_inner(4, 0L, tmp, 2);
	}
	write_inner
	(
	    0,
	    (record.get_address() >> 1) & 0x0000FFFF,
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
	if (record.get_address() == 0)
	    break;
	srec_record::encode_big_endian(tmp, record.get_address() >> 1, 4);
	write_inner(5, 0L, tmp, 4);
	break;

    case srec_record::type_unknown:
	fatal_error("can't write unknown record type");
	break;
    }
}


void
srec_output_file_intel16::line_length_set(int n)
{
    //
    // Given the number of characters, figure the maximum number of
    // data baytes.
    //
    n = ((n - 11) / 2) & ~1;

    //
    // Constrain based on the file format.
    // (255*2 is the largest that will fit in the data size field)
    //
    if (n < 2)
	n = 2;
    else if (n > 255*2)
	n = 255*2;

    // 
    // An additional constraint is the size of the srec_record
    // data buffer.
    //
    if (n > srec_record::max_data_length)
	n = (srec_record::max_data_length & ~1);
    pref_block_size = n;
}


void
srec_output_file_intel16::address_length_set(int)
{
    // ignore
}


int
srec_output_file_intel16::preferred_block_size_get()
	const
{
    return pref_block_size;
}