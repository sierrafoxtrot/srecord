//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998, 1999, 2001-2003 Peter Miller;
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
// MANIFEST: functions to impliment the srec_output_file_tektronix class
//

#pragma implementation "srec_output_file_tektronix"

#include <srec/output/file/tektronix.h>
#include <srec/record.h>


srec_output_file_tektronix::srec_output_file_tektronix() :
    srec_output_file("-"),
    pref_block_size(32)
{
}


srec_output_file_tektronix::srec_output_file_tektronix(const char *filename) :
    srec_output_file(filename),
    pref_block_size(32)
{
}


srec_output_file_tektronix::~srec_output_file_tektronix()
{
    // make sure terminator is written
}


void
srec_output_file_tektronix::put_nibble(int n)
{
    srec_output_file::put_nibble(n);
    checksum_add(n & 15);
}


void
srec_output_file_tektronix::put_byte(unsigned char n)
{
    // This differs from srec_output_file::put_byte only in that it
    // doesn't add to the checksum.
    put_nibble(n >> 4);
    put_nibble(n);
}


void
srec_output_file_tektronix::write_inner(unsigned long address,
    const void *data, int data_nbytes)
{
    //
    // Make sure the line is not too long.
    //
    if (data_nbytes >= 256)
	fatal_error("data length (%d) too long", data_nbytes);

    //
    // Emit the line as hexadecimal text.
    //
    put_char('/');
    unsigned char tmp[2];
    srec_record::encode_big_endian(tmp, address, 2);
    checksum_reset();
    put_byte(tmp[0]);
    put_byte(tmp[1]);
    put_byte(data_nbytes);
    put_byte(checksum_get());
    if (data_nbytes)
    {
	checksum_reset();
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
	// This format can't do header reocrds
	break;

    case srec_record::type_data:
	if (record.get_length() == 0)
		break; // ignore
	if (record.get_address() + record.get_length() > (1UL << 16))
	{
	    fatal_error
	    (
	       	"data address (0x%lX..0x%lX) too large",
	       	record.get_address(),
	       	record.get_address() + record.get_length() - 1
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
	// ignore
	break;

    case srec_record::type_start_address:
	if (data_only_flag)
    	    break;
	if (record.get_address() >= (1UL << 16))
	{
    	    fatal_error
    	    (
       		"start address (%08lX) too large",
       		record.get_address()
    	    );
	}
	write_inner(record.get_address(), 0, 0);
	break;

    case srec_record::type_unknown:
	fatal_error("can't write unknown record type");
    }
}


void
srec_output_file_tektronix::line_length_set(int n)
{
    //
    // Given the number of characters, figure the maximum number of
    // data baytes.
    //
    n = (n - 11) / 2;

    //
    // Constrain based on the file format.
    // (255 is the largest that will fit in the data length field)
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
srec_output_file_tektronix::address_length_set(int)
{
    // ignore (this is a 16-bit format)
}


int
srec_output_file_tektronix::preferred_block_size_get()
    const
{
    return pref_block_size;
}
